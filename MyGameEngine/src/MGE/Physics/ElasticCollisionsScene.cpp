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
		m_CirclePhyicsObjectContainer.insert(std::make_pair(count, CirclePhyicsObject(
			Vec2_Physics{ 0.0f, 9.0f },
			count,
			Vec2_Physics{ 15.0f, 0.0f }
		)));

		// TODO: aabb collision detection
		m_CirclePhyicsObjectContainer[count].SetMotionLimit(m_SceneWidth, m_SceneHeight);
		m_PhysicsObjects.push_back(&m_CirclePhyicsObjectContainer[count]);
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
			PROFILE_SCOPE("Collision handle");
			for (int i = 0; i < 8; i++)
			{
				for (auto& [i, j] : m_CirclePhyicsObjectContainer)
				{
					j.OnUpdate(ts / 8.0f);
				}
				
				if (m_NumberOfObjects < 600)
				{
					FindCollisions(); //single thread version
				}
				else 
				{
					FindCollisions_mutithread_Call();
				}
				
				// FindCollisions_BrutalForce();
				for (auto& [i, j] : m_CirclePhyicsObjectContainer)
				{
					j.ApplyMotionLimit();
				}
			}
		}
		{
			PROFILE_SCOPE("Drawing");
			for (auto& [i, j] : m_CirclePhyicsObjectContainer)
			{
				j.DrawPhysicsObject();
			}
		}
		
		for (auto i : m_Grid) 
		{
			for (auto j : i) 
			{
				j->object_id.clear();
			}
		}
	}

	void PhysicsScene::ElasticCollisions(CirclePhyicsObject* i, CirclePhyicsObject* j)
	{	
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

	void PhysicsScene::FindCollisions_mutithread(int start_X, int start_Y)
	{
		for (int i = start_Y; i <= (int)(start_Y + m_SceneHeight); i++)
		{
			for (int j = start_X; j <= (int)(start_X + m_SceneWidth / 2.0f); j++)
			{
				/* loop through the 2-D grid vector */
				if (m_Grid[i][j]->object_id.size() == 0)
					continue; 

				for (auto uid : m_Grid[i][j]->object_id)
				{
					for(int a = -1; a <= 1; a ++)
					{
						for(int b = -1; b <= 1; b++)
						{
							if (m_Grid[i + a][j + b]->object_id.size() == 0)
							{
								continue;
							}

							if (mathter::Length(m_CirclePhyicsObjectContainer[uid].GetVelocity()) < 0.1f && m_CirclePhyicsObjectContainer[uid].GetSkipFrame() < 4)
							{
								m_CirclePhyicsObjectContainer[uid].AddSkipFrame();
								continue;
							}

							for (auto uid_1 : m_Grid[i + a][j + b]->object_id)
							{
								if(uid_1 == uid)
									continue;

								Vec2_Physics hit_distance = Vec2_Physics(m_CirclePhyicsObjectContainer[uid].GetPosition() - m_CirclePhyicsObjectContainer[uid_1].GetPosition());
								if (mathter::Length(hit_distance) < m_CirclePhyicsObjectContainer[uid].GetRadius() * 2.0f)
								{
									ElasticCollisions(&m_CirclePhyicsObjectContainer[uid], &m_CirclePhyicsObjectContainer[uid_1]);
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
				if (m_Grid[i][j]->object_id.size() == 0)
					continue; 

				for (auto uid : m_Grid[i][j]->object_id)
				{
					for(int a = -1; a <= 1; a ++)
					{
						for(int b = -1; b <= 1; b++)
						{
							if (m_Grid[i + a][j + b]->object_id.size() == 0)
								continue;

							for (auto uid_1 : m_Grid[i + a][j + b]->object_id)
							{
								if(uid_1 == uid)
									continue;
								
								if (mathter::Length(m_CirclePhyicsObjectContainer[uid].GetVelocity()) < 0.1f && m_CirclePhyicsObjectContainer[uid].GetSkipFrame() < 4)
								{
									m_CirclePhyicsObjectContainer[uid].AddSkipFrame();
									continue;
								}

								Vec2_Physics hit_distance = Vec2_Physics(m_CirclePhyicsObjectContainer[uid].GetPosition() - m_CirclePhyicsObjectContainer[uid_1].GetPosition());
								if (mathter::Length(hit_distance) < m_CirclePhyicsObjectContainer[uid].GetRadius() * 2.0f)
								{
									m_CirclePhyicsObjectContainer[uid].ResetSkipFrame();
									ElasticCollisions(&m_CirclePhyicsObjectContainer[uid], &m_CirclePhyicsObjectContainer[uid_1]);
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
		for(auto [i, object] : m_CirclePhyicsObjectContainer)
		{
			int x = (int)(object.GetPosition().x + m_SceneWidth + 1);
			int y = (int)(object.GetPosition().y + m_SceneHeight + 1);

			y = (y > 5) ? 5 : y;

			m_Grid[y][x]->object_id.push_back(i);
		}
	}

	void PhysicsScene::FindCollisions_mutithread_Call()
	{	
		std::vector<std::thread> threads;
		int numThreads = 8;

		threads.push_back(std::thread(&PhysicsScene::FindCollisions_mutithread, this,
			(int)( m_SceneWidth + 1),
			(int)(m_SceneHeight / 2.0f + 1.0f)));

		for (int i = 0; i < 4; ++i) { 
			threads.push_back(std::thread(&PhysicsScene::FindCollisions_mutithread, this,
				(int)(i * m_SceneWidth / (numThreads / 4) + 1),
				1 
			));
		}
		for (auto& thread : threads) {
			PROFILE_SCOPE("thread join");
			thread.join();
		}
	}

	void PhysicsScene::FindCollisions_BrutalForce()
	{
		// for (int i = 0; i < m_NumberOfObjects - 1; i++)
		// {
		// 	for (int j = i + 1; j < m_NumberOfObjects; j++)
		// 	{
		// 		Vec2 hit_distance = Vec2(m_PhysicsObjects[i].GetPosition() - m_PhysicsObjects[j].GetPosition());
		// 		if (mathter::Length(hit_distance) < 0.2f)
		// 		{
		// 			ElasticCollisions(&m_PhysicsObjects[i], &m_PhysicsObjects[j]);
		// 		}
		// 	}
		// }
	}

}