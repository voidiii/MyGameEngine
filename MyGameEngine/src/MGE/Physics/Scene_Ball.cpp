#include "MGEpch.h"
#include "Scene_Ball.h"
#include <thread>

std::mutex mtx;

namespace MGE {
	
	PhysicsScene::PhysicsScene(float height, float width, int numberOfObjects)
		: m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(0)
	{
		m_int_width = f_toint32(m_SceneWidth);
		m_int_height = f_toint32(m_SceneHeight);
		
		m_Grid.reserve(m_int_height * 2 + 3);
		CreateObjects(500);
		GridManage();

		m_ThreadPool.Start();
		m_CirclePhyicsObjectContainer.reserve(10000);
	}

	void PhysicsScene::CreateObjects(int count)
	{
		srand(time(0));

		for (int i = 0; i < count; i++)
		{
			m_CirclePhyicsObjectContainer.emplace_back( CirclePhyicsObject(
				std::move(Vec2_Physics{ 					
					rand() * ((m_SceneWidth - 2) * 2.0f / RAND_MAX) - m_SceneWidth + 2, 
					rand() * ((m_SceneHeight - 2) * 2.0f / RAND_MAX) - m_SceneHeight + 2 
				}),
				i,
				std::move(Vec2_Physics{ 0.0f, 0.0f })
			));

			// TODO: aabb collision detection
			m_CirclePhyicsObjectContainer[i].SetMotionLimit(m_SceneWidth, m_SceneHeight);
			AddNumberOfObjects();
		}
	}

	void PhysicsScene::OnUpdate(Timestep ts)
	{
		m_ProfileResults.clear();
		m_ThreadPool.resume();

		{
			PROFILE_SCOPE("Collision handle");
			for (int i = 0; i < 8; i++)
			{	
				for (const auto& grid : m_Grid)
				{
					for (const auto& j : grid)
					{
						j->object_id.clear();
					}
				}

				SetUpGrid();
				SetUpDensity();
				FindCollisions(); //single thread version
				// FindCollisions_mutithread_Call();
				// FindCollisions_BrutalForce();
				ApplyPressue();
				for (auto& j : m_CirclePhyicsObjectContainer)
				{
					//j.OnUpdate(ts / 8.0f);
					j.OnUpdate_Verlet(ts/8.0f);
				}
				ClearPressure();
			}
		}
		m_ThreadPool.pause();
		{
			PROFILE_SCOPE("Drawing");
			for (auto& j : m_CirclePhyicsObjectContainer)
			{
				j.DrawPhysicsObject();
			}
		}
	}


	void PhysicsScene::ElasticCollisions_Verlet(CirclePhyicsObject* i, CirclePhyicsObject* j)
	{	
		//mtx.lock(); // lock the mutex
		Vec2_Physics hit_distance = Vec2_Physics(i->GetPosition() - j->GetPosition());

		if (mathter::Length(hit_distance) < 0.0001f) return;

		Vec2_Physics hit_direction = mathter::Normalize(hit_distance);

		i->SetCurrentPosition(i->GetPosition() + (i->GetRadius() * 2.0f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
		j->SetCurrentPosition(j->GetPosition() - (j->GetRadius() * 2.0f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
		//mtx.unlock();
	}

	void PhysicsScene::FindCollisions_GridHandling(int i, int j)
	{
		for (const auto& uid : m_Grid[i][j]->object_id)
		{
			for (int a = -1; a <= 1; a++)
			{
				for (int b = -1; b <= 1; b++)
				{
					if (m_Grid[i + a][j + b]->object_id.size() == 0)
					{
						continue;
					}
			
					for (const auto& uid_1 : m_Grid[i + a][j + b]->object_id)
					{
						if (uid_1 == uid)
							continue;
			
						Vec2_Physics hit_distance = Vec2_Physics(m_CirclePhyicsObjectContainer[uid].GetPosition() - m_CirclePhyicsObjectContainer[uid_1].GetPosition());

						if (mathter::Length(hit_distance) <  m_CirclePhyicsObjectContainer[uid].GetRadius() * 2.0f)
						{
							ElasticCollisions_Verlet(&m_CirclePhyicsObjectContainer[uid], &m_CirclePhyicsObjectContainer[uid_1]);
						}
					}
				}
			}
		}
	}

	void PhysicsScene::GridManage()
	{
		for (int i = -m_int_height - 1; i <= m_int_height + 1; i++)
		{
			std::vector<Ref<Grid>> temp;
			temp.reserve(m_int_width * 2 + 3);
			for (int j = -m_int_width - 1; j <= m_int_width + 1; j++)
			{
				temp.emplace_back(CreateRef<Grid>(
					i, j
				));
			}
			m_Grid.push_back(temp);
		}

		SetUpGrid();
	}

	/*
		The grid is supposed to be one grid bigger than the scene so that 
		the objects on the edge of the scene can be dealt with in the collision-detection loop.
	*/
	void PhysicsScene::SetUpGrid() 
	{
		for(const auto& object : m_CirclePhyicsObjectContainer)
		{
			int x = f_toint32(object.GetPosition().x + m_SceneWidth + 1);
			int y = f_toint32(object.GetPosition().y + m_SceneHeight + 1);

			m_Grid[y][x]->object_id.emplace_back(object.GetUID());
		}
	}
	
	void PhysicsScene::FindCollisions()
	{
		for (int i = 1; i <= (m_int_height * 2); i++)
		{
			for (int j = 1; j <= (m_int_width * 2); j++)
			{
				/* loop through the 2-D grid vector */
				if (m_Grid[i][j]->object_id.size() == 0)
					continue; 

				FindCollisions_GridHandling(i, j);
			}
		}
	}

	void PhysicsScene::SetUpDensity()
	{
		for (auto& j : m_CirclePhyicsObjectContainer)
		{
			j.SetDensity(CaculateDensity(j.GetPosition(), j.GetUID()));
		}
	}

	float PhysicsScene::CaculateDensity(Vec2_Physics position, int id)
	{
		float density = 0.0f;
		
		int x = f_toint32(position.x + m_SceneWidth + 1);
		int y = f_toint32(position.y + m_SceneHeight + 1);

		for (int a = -3; a <= 3; a++)
		{
			for (int b = -3; b <= 3; b++)
			{
				if ((y + a < 0) || (y + a) >= m_Grid.size() || (x + b < 0) || (x + b >= m_Grid[0].size()))
				{
					continue;
				}
				if (m_Grid[y + a][x + b]->object_id.size() == 0)
				{
					continue;
				}

				for (const auto& uid : m_Grid[y + a][x + b]->object_id)
				{
					float dst = mathter::Length(position - m_CirclePhyicsObjectContainer[uid].GetPosition());
					float influence = SmoothingKernel(m_SmoothingRadius, dst);
					density += influence;
				}
			}
		}
		return density;
	}

	Vec2_Physics PhysicsScene::CaculatePressureForce(Vec2_Physics position, int id)
	{
		Vec2_Physics pressureForce = Vec2_Physics(0.0f);

		int x = f_toint32(position.x + m_SceneWidth + 1);
		int y = f_toint32(position.y + m_SceneHeight + 1);

		for (int a = -3; a <= 3; a++)
		{
			for (int b = -3; b <= 3; b++)
			{
				if ((y + a < 0) || (y + a) >= m_Grid.size() || (x + b < 0) || (x + b >= m_Grid[0].size()))
				{
					continue;
				}
				if (m_Grid[y + a][x + b]->object_id.size() == 0)
				{
					continue;
				}

				for (const auto& uid : m_Grid[y + a][x + b]->object_id)
				{
					if (uid == id) continue;

					Vec2_Physics dir = position - m_CirclePhyicsObjectContainer[uid].GetPosition();
					float dst = mathter::Length(dir);
					float random = rand() * (6.28 / RAND_MAX);
					dir = dst == 0 ? Vec2_Physics(cos(random), sin(random)) : dir / dst;

					float slope = SmoothingKernelDerivative(dst, m_SmoothingRadius);
					float density = m_CirclePhyicsObjectContainer[uid].GetDensity();
					float temp = -ConvertDensityToPressure(density);

					pressureForce += temp * dir * slope / density;
				}
			}
		}
		return pressureForce;
	}

	void PhysicsScene::ApplyPressue()
	{
		for (int i = 1; i <= (m_int_height * 2); i++)
		{
			for (int j = 1; j <= (m_int_width * 2); j++)
			{
				/* loop through the 2-D grid vector */
				if (m_Grid[i][j]->object_id.size() == 0)
					continue;

				for (const auto& uid : m_Grid[i][j]->object_id)
				{
					m_CirclePhyicsObjectContainer[uid].AddPressureAcceleration(CaculatePressureForce(m_CirclePhyicsObjectContainer[uid].GetPosition(), m_CirclePhyicsObjectContainer[uid].GetUID()) / m_CirclePhyicsObjectContainer[uid].GetDensity());
				}
			}
		}
	}

	void PhysicsScene::ClearPressure()
	{
		for (auto& j : m_CirclePhyicsObjectContainer)
		{
			j.ClearPressureAcceleration();
		}
	}

	void PhysicsScene::FindCollisions_mutithread_Call()
	{	
		for(int j = 1; j < m_int_width * 2 + 2; j++)
		{ 	
			m_ThreadPool.QueueJob([this, j]() {
				for (int i = 1; i < m_int_height * 2 + 2; i ++)
				{
					FindCollisions_GridHandling(i, j);
				}
			});
		}

		while (m_ThreadPool.busy()) 
		{
			continue;
		}
	}

	void PhysicsScene::FindCollisions_BrutalForce()
	{
		for (auto& object_i : m_CirclePhyicsObjectContainer)
		{
			for (auto& object_j : m_CirclePhyicsObjectContainer)
			{
				if (object_i.GetUID() == object_j.GetUID()) continue;

				float X = object_i.GetPosition().x - object_j.GetPosition().x;
				float Y = object_i.GetPosition().y - object_j.GetPosition().y;


				float hit_distance = X*X + Y*Y;
				if (hit_distance < 1.0f)
				{
					ElasticCollisions_Verlet(&object_i, &object_j);
				}
			}
		}
	}

	void PhysicsScene::FindCollisions_GridHandling_CUDA(int i, int j)
	{
		std::vector<glm::vec2> surrounding_objects_temp;
		std::vector<glm::vec2> center_object_position_temp;
		std::vector<glm::vec2> results_temp;
		surrounding_objects_temp.reserve(5000);
		center_object_position_temp.reserve(5000);
		results_temp.reserve(5000);

		std::vector<int> center_object_id;
		std::vector<int> surrounding_object_id;
		size_t bytes;

		bytes = 5000 * sizeof(glm::vec2);

		int size = 0;


		for (const auto& uid : m_Grid[i][j]->object_id)
		{
			for (int a = -1; a <= 1; a++)
			{
				for (int b = -1; b <= 1; b++)
				{
					if (m_Grid[i + a][j + b]->object_id.size() == 0)
					{
						continue;
					}
					for (const auto& uid_1 : m_Grid[i + a][j + b]->object_id)
					{
						surrounding_objects_temp.emplace_back(glm::vec2(m_CirclePhyicsObjectContainer[uid_1].GetPosition().x, m_CirclePhyicsObjectContainer[uid_1].GetPosition().y));
						center_object_position_temp.emplace_back(glm::vec2(m_CirclePhyicsObjectContainer[uid].GetPosition().x, m_CirclePhyicsObjectContainer[uid].GetPosition().y));
						center_object_id.push_back(uid);
						surrounding_object_id.push_back(uid_1);
						size++;
					}
				}
			}

			//for (int a = -1; a <= 1; a++)
			//{
			//	for (int b = -1; b <= 1; b++)
			//	{
			//		if (m_Grid[i + a][j + b]->object_id.size() == 0)
			//		{
			//			continue;
			//		}
			//
			//		for (const auto& uid_1 : m_Grid[i + a][j + b]->object_id)
			//		{
			//			if (uid_1 == uid)
			//				continue;
			//
			//			Vec2_Physics hit_distance = Vec2_Physics(m_CirclePhyicsObjectContainer[uid].GetPosition() - m_CirclePhyicsObjectContainer[uid_1].GetPosition());
			//
			//			//float X = m_CirclePhyicsObjectContainer[uid].GetPosition().x - m_CirclePhyicsObjectContainer[uid_1].GetPosition().x;
			//			//float Y = m_CirclePhyicsObjectContainer[uid].GetPosition().y - m_CirclePhyicsObjectContainer[uid_1].GetPosition().y;
			//			//
			//			//
			//			//float hit_distance = X * X + Y * Y;
			//			if (glm::length(hit_distance) <  m_CirclePhyicsObjectContainer[uid].GetRadius() * 2.0f)
			//			{
			//				ElasticCollisions_Verlet(&m_CirclePhyicsObjectContainer[uid], &m_CirclePhyicsObjectContainer[uid_1]);
			//			}
			//		}
			//	}
			//}
		}

		std::memcpy(m_surrounding_objects, surrounding_objects_temp.data(), size * sizeof(glm::vec2));
		std::memcpy(m_center_object_position, center_object_position_temp.data(), size * sizeof(glm::vec2));

		// Copy data to the device
		// int error = cudaMemcpy(d_a, center_object_position.data(), bytes, cudaMemcpyHostToDevice);
		// error = cudaMemcpy(d_b, surrounding_objects.data(), bytes, cudaMemcpyHostToDevice);

		ElasticCollisions_Verlet_GPU_Jumper(m_surrounding_objects, m_center_object_position, m_results, size);
		cudaDeviceSynchronize();

		// results.resize(surrounding_objects.size());
		// cudaMemcpy(results.data(), d_c, bytes, cudaMemcpyDeviceToHost);

		std::memcpy(results_temp.data(), m_results, size * sizeof(glm::vec2));
		results_temp.resize(size);


		for (int i = 0; i < size; i++)
		{
			Vec2_Physics new_position = Vec2_Physics(m_CirclePhyicsObjectContainer[center_object_id[i]].GetPosition().x + results_temp[i].x, m_CirclePhyicsObjectContainer[center_object_id[i]].GetPosition().y + results_temp[i].y);
			m_CirclePhyicsObjectContainer[center_object_id[i]].SetCurrentPosition(new_position);
		}

		cudaMemset(m_results, 0, size * sizeof(glm::vec2));
		cudaMemset(m_surrounding_objects, 0, size * sizeof(glm::vec2));
		cudaMemset(m_center_object_position, 0, size * sizeof(glm::vec2));
	}

	void PhysicsScene::ElasticCollisions(CirclePhyicsObject* i, CirclePhyicsObject* j)
	{	
		// mtx.lock(); // lock the mutex
		Vec2_Physics hit_distance = Vec2_Physics(i->GetPosition() - j->GetPosition());
		Vec2_Physics hit_direction = mathter::Normalize(hit_distance);

		float temp = mathter::Dot(Vec2_Physics(i->GetVelocity() - j->GetVelocity()), hit_distance);
		
		// float mass_i = i->GetMass();
		// float mass_j = j->GetMass();
		
		float delta_x_square = mathter::Length(hit_distance) * mathter::Length(hit_distance);
		
		// Vec2 new_v_temp = ((2.0f * mass_j) / (mass_i + mass_j)) * (temp) / (delta_x_square) * (hit_distance); // ignore mass for now
		Vec2_Physics new_v_temp = (temp) / (delta_x_square) * (hit_distance);
		
		i->UpdateVelocity(-new_v_temp);
		j->UpdateVelocity(new_v_temp);

		i->UpdatePosition( (i->GetRadius() * 2.0f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
		j->UpdatePosition(-(i->GetRadius() * 2.0f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
	}
}