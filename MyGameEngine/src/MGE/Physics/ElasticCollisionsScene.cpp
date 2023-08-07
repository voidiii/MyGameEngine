#include "MGEpch.h"
#include "ElasticCollisionsScene.h"
#include <thread>

std::mutex mtx;

namespace MGE {
	
	PhysicsScene::PhysicsScene(float height, float width, int numberOfObjects)
		: m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(0)
	{
		m_int_width = f_toint32(m_SceneWidth);
		m_int_height = f_toint32(m_SceneHeight);
		
		m_Grid.reserve(m_int_height * 2 + 3);
		GridManage();

		m_ThreadPool.Start();
		m_CirclePhyicsObjectContainer.reserve(5000);
	}

	void PhysicsScene::CreateObjects(int& count)
	{
		m_CirclePhyicsObjectContainer.emplace_back( CirclePhyicsObject(
			std::move(Vec2_Physics{ 0.0f, 45.0f - 2 * (float)(count % 4)}),
			count,
			std::move(Vec2_Physics{ 15.0f, 0.0f })
		));

		// TODO: aabb collision detection
		m_CirclePhyicsObjectContainer[count].SetMotionLimit(m_SceneWidth, m_SceneHeight);
		//m_PhysicsObjects.push_back(&m_CirclePhyicsObjectContainer[count]);
	}

	void PhysicsScene::OnUpdate(Timestep ts)
	{
		m_ProfileResults.clear();
		
		static int count = 0;
		static int frame = 0;

		if (frame == 4)
		{
			frame = 0;
			for (int i = 0; i < 4; i++)
			{
				CreateObjects(count);
				AddNumberOfObjects();
				count++;
			}
		}
		frame++;
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
				// FindCollisions(); //single thread version
				FindCollisions_mutithread_Call();
				// FindCollisions_BrutalForce();
				for (auto& j : m_CirclePhyicsObjectContainer)
				{
					j.OnUpdate_Verlet(ts/8.0f );
				}
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

	void PhysicsScene::FindCollisions()
	{
		
		for (int i = 1; i <= (m_int_height * 2); i++)
		{
			for (int j = 1; j <= (m_int_height * 2); j++)
			{
				/* loop through the 2-D grid vector */
				if (m_Grid[i][j]->object_id.size() == 0)
					continue; 

				for (const auto& uid : m_Grid[i][j]->object_id)
				{
					for(int a = -1; a <= 1; a ++)
					{
						for(int b = -1; b <= 1; b++)
						{
							if (m_Grid[i + a][j + b]->object_id.size() == 0)
								continue;

							for (const auto& uid_1 : m_Grid[i + a][j + b]->object_id)
							{
								if(uid_1 == uid)
									continue;
								
								// if (mathter::Length(m_CirclePhyicsObjectContainer[uid].GetVelocity()) < 0.1f && m_CirclePhyicsObjectContainer[uid].GetSkipFrame() < 4)
								// {
								// 	m_CirclePhyicsObjectContainer[uid].AddSkipFrame();
								// 	continue;
								// }

								Vec2_Physics hit_distance = Vec2_Physics(m_CirclePhyicsObjectContainer[uid].GetPosition() - m_CirclePhyicsObjectContainer[uid_1].GetPosition());
								if (mathter::Length(hit_distance) < m_CirclePhyicsObjectContainer[uid].GetRadius() * 2.0f)
								{
									//m_CirclePhyicsObjectContainer[uid].ResetSkipFrame();
									ElasticCollisions_Verlet(&m_CirclePhyicsObjectContainer[uid], &m_CirclePhyicsObjectContainer[uid_1]);
								}
							}
						}
					}
				}
			}
		}
	}

	void PhysicsScene::FindCollisions_GridHandling(int i, int j)
	{
		// if (i >= m_Grid.size() - 1 || j >= m_Grid[0].size() - 1) return;
		
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

						//Vec2_Physics hit_distance = Vec2_Physics(m_CirclePhyicsObjectContainer[uid].GetPosition() - m_CirclePhyicsObjectContainer[uid_1].GetPosition());

						float X = m_CirclePhyicsObjectContainer[uid].GetPosition().x - m_CirclePhyicsObjectContainer[uid_1].GetPosition().x;
						float Y = m_CirclePhyicsObjectContainer[uid].GetPosition().y - m_CirclePhyicsObjectContainer[uid_1].GetPosition().y;
						
						
						float hit_distance = X * X + Y * Y;
						if (hit_distance <  m_CirclePhyicsObjectContainer[uid].GetRadius() * 2.0f)
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

			// y = (y > 5) ? 5 : y;

			m_Grid[y][x]->object_id.emplace_back(object.GetUID());
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

		//for (int j = 1; j < m_Grid[0].size() - 1; j++)
		//{
		//	m_ThreadPool.QueueJob([this, j]() {
		//		for (int i = 2; i < m_Grid.size() - 1; i += 3)
		//		{
		//			FindCollisions_GridHandling(i, j);
		//		}
		//		});
		//}
		
		//while (m_ThreadPool.busy())
		//{
		//	continue;
		//}
		
		//for (int j = 1; j < m_Grid[0].size() - 1; j++)
		//{
		//	m_ThreadPool.QueueJob([this, j]() {
		//		for (int i = 3; i < m_Grid.size() - 1; i += 3)
		//		{
		//			FindCollisions_GridHandling(i, j);
		//		}
		//		});
		//}
		//
		//while (m_ThreadPool.busy())
		//{
		//	continue;
		//}
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

}