#include "MGEpch.h"
#include "ElasticCollisionsScene.h"
#include <thread>

namespace MGE {
	
	PhysicsScene::PhysicsScene(float height, float width, int numberOfObjects)
		: m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(0)
	{
		m_Grid.reserve((int)m_SceneHeight * 2 + 3);
		GridManage();
	}

	void PhysicsScene::CreateObjects(int& count)
	{
		m_CirclePhyicsObjectContainer.insert(std::make_pair(count, CreateRef<CirclePhyicsObject>(
			Vec2{ 3.0f, 3.0f },
			Vec4{ 0.2f, 0.3f, 0.8f, 1.0f },
			count,
			Vec2{ 5.0f, 0.0f }
		)));

		// TODO: aabb collision detection
		m_CirclePhyicsObjectContainer[count]->SetMotionLimit(m_SceneWidth, m_SceneHeight);
		m_PhysicsObjects.push_back(m_CirclePhyicsObjectContainer[count]);
	}

	void PhysicsScene::OnUpdate(Timestep ts)
	{
		m_ProfileResults.clear();
		
		static int count = 0;
		static int frame = 0;

		if (frame == 5)
		{
			frame = 0;
			CreateObjects(count);
			AddNumberOfObjects();
			count++;
		}
		frame++;
		SetUpGrid();
		
		{
			PROFILE_SCOPE("Sandbox2D::FindCollisions_mutithread_Call");
			for (int i = 0; i < 4; i++)
			{
				for (auto i : m_PhysicsObjects)
				{
					i->OnUpdate(ts / 4.0f);
				}
				// FindCollisions(); //single thread version
				FindCollisions_mutithread_Call();
				// FindCollisions_BrutalForce();
				for (auto i : m_PhysicsObjects)
				{
					i->ApplyMotionLimit();
				}
			}
		}
		{
			PROFILE_SCOPE("Sandbox2D::Drawing");
			for (auto i : m_PhysicsObjects)
			{
				i->DrawPhysicsObject();
			}
		}
		
		for (auto i : m_Grid) 
		{
			for (auto j : i) 
			{
				j->GridObjects->clear();
			}
		}
	}

	void PhysicsScene::ElasticCollisions(Ref<MGE::CirclePhyicsObject> i, Ref<MGE::CirclePhyicsObject> j) 
	{	
		Vec2 hit_distance = Vec2(i->GetPosition() - j->GetPosition());
		Vec2 hit_direction = mathter::Normalize(hit_distance);

		float temp = mathter::Dot(Vec2(i->GetVelocity() - j->GetVelocity()), hit_distance);
		
		// float mass_i = i->GetMass();
		// float mass_j = j->GetMass();
		
		float delta_x_square = mathter::Length(hit_distance) * mathter::Length(hit_distance);
		
		// Vec2 new_v_temp = ((2.0f * mass_j) / (mass_i + mass_j)) * (temp) / (delta_x_square) * (hit_distance); // ignore mass for now
		Vec2 new_v_temp = (temp) / (delta_x_square) * (hit_distance);
		
		i->UpdateVelocity(-new_v_temp);
		j->UpdateVelocity(new_v_temp);

		i->UpdatePosition((0.2f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
		j->UpdatePosition(-(0.2f - mathter::Length(hit_distance)) / 2.0f * hit_direction);
	}

	void PhysicsScene::FindCollisions_mutithread(int start_X, int start_Y)
	{
		for (int i = start_Y; i <= (int)(start_Y + m_SceneHeight); i++)
		{
			for (int j = start_X; j <= (int)(start_X + m_SceneWidth / 2.0f); j++)
			{
				/* loop through the 2-D grid vector */
				if (m_Grid[i][j]->GridObjects->size() == 0) 
					continue; 

				for (auto& [key, k] : *m_Grid[i][j]->GridObjects)
				{
					for(int a = -1; a <= 1; a ++)
					{
						for(int b = -1; b <= 1; b++)
						{
							if (m_Grid[i + a][j + b]->GridObjects->size() == 0) 
								continue;

							for (auto& [key_1, k_1] : *m_Grid[i + a][j + b]->GridObjects)
							{
								if(k == k_1)  
									continue;

								Vec2 hit_distance = Vec2(k->GetPosition() - (k_1)->GetPosition());
								if (mathter::Length(hit_distance) < 0.2f)
								{
									ElasticCollisions(k, (k_1));
								}
							}
						}
					}
				}
			}
		}
	}

	void PhysicsScene::FindCollisions()
	{
		
		for (int i = 1; i <= (int)(m_SceneHeight * 2); i++)
		{
			for (int j = 1; j <= (int)(m_SceneWidth * 2); j++)
			{
				/* loop through the 2-D grid vector */
				if (m_Grid[i][j]->GridObjects->size() == 0) 
					continue; 

				for (auto& [key, k] : *m_Grid[i][j]->GridObjects)
				{
					for(int a = -1; a <= 1; a ++)
					{
						for(int b = -1; b <= 1; b++)
						{
							if (m_Grid[i + a][j + b]->GridObjects->size() == 0) 
								continue;

							for (auto& [key_1, k_1] : *m_Grid[i + a][j + b]->GridObjects)
							{
								if(k == k_1)  
									continue;

								Vec2 hit_distance = Vec2(k->GetPosition() - (k_1)->GetPosition());
								if (mathter::Length(hit_distance) < 0.2f)
								{
									ElasticCollisions(k, (k_1));
								}
							}
						}
					}
				}
			}
		}
	}

	void PhysicsScene::GridManage()
	{
		for (int i = -(int)m_SceneHeight - 1; i <= (int)m_SceneHeight + 1; i++)
		{
			std::vector<Ref<Grid>> temp;
			temp.reserve((int)m_SceneWidth * 2 + 3);
			for (int j = -(int)m_SceneWidth - 1; j <= (int)m_SceneWidth + 1; j++)
			{
				temp.emplace_back(CreateRef<Grid>(
					i, j, CreateRef<std::unordered_map<int, Ref<CirclePhyicsObject>>>()
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
		PROFILE_SCOPE("Sandbox2D::SetUpGrid");
		for(auto object : m_PhysicsObjects)
		{
			int x = (int)(object->GetPosition().x + m_SceneWidth + 1);
			int y = (int)(object->GetPosition().y + m_SceneHeight + 1);

			y = (y > 5) ? 5 : y;

			m_Grid[y][x]->GridObjects->insert(
				std::make_pair(object->GetUID(), object)
			);
		}
	}

	void PhysicsScene::FindCollisions_mutithread_Call()
	{	
		std::vector<std::thread> threads;
		int numThreads = 8;

		for (int i = 0; i < numThreads / 2; ++i) {
			threads.push_back(std::thread(&PhysicsScene::FindCollisions_mutithread, this,
				(int)(i * m_SceneWidth / (numThreads / 4) + 1),
				(int)(m_SceneHeight / 2.0f + 1.0f)));
			threads.push_back(std::thread(&PhysicsScene::FindCollisions_mutithread, this,
				(int)(i * m_SceneWidth / (numThreads / 4) + 1),
				1));
		}
		for (auto& thread : threads) {
			thread.join();
		}
	}

	void PhysicsScene::FindCollisions_BrutalForce()
	{
		for (int i = 0; i < m_NumberOfObjects - 1; i++)
		{
			for (int j = i + 1; j < m_NumberOfObjects; j++)
			{
				Vec2 hit_distance = Vec2(m_PhysicsObjects[i]->GetPosition() - m_PhysicsObjects[j]->GetPosition());
				if (mathter::Length(hit_distance) < 0.2f)
				{
					ElasticCollisions(m_PhysicsObjects[i], m_PhysicsObjects[j]);
				}
			}
		}
	}

}