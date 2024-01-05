#include "MGEpch.h"
#include "Scene_CollisionDetection.h"

#define GJK 1

namespace MGE {

	typedef std::map<CollisionKey, Arbiter>::iterator ArbIter;
	typedef std::pair<CollisionKey, Arbiter> ArbPair;

	Vec2_Physics support(const std::vector<Vec2_Physics> Convexhull, const Vec2_Physics direction)
	{
		Vec2_Physics result = Convexhull[0];
		float max = mathter::Dot(Convexhull[0], direction);
		for (auto i : Convexhull)
		{
			float temp = mathter::Dot(i, direction);
			if (max < temp)
			{
				result = i;
				max = temp;
			}
		}

		return result;
	}

	bool lineCase(std::vector<Vec2_Physics>& simplex, Vec2_Physics& direction)
	{
		Vec2_Physics A = simplex[0];
		Vec2_Physics B = simplex[1];
		Vec2_Physics AB = B - A;
		Vec2_Physics AO = -A;

		Vec3 temp = mathter::Cross(mathter::Cross(Vec3(AB.x, AB.y, 0.0f), Vec3(AO.x, AO.y, 0.0f)), Vec3(AB.x, AB.y, 0.0f));

		direction.x = temp.x;
		direction.y = temp.y;

		return false;
	}

	bool triangleCase(std::vector<Vec2_Physics>& simplex, Vec2_Physics& direction)
	{
		Vec2_Physics C = simplex[0];
		Vec2_Physics B = simplex[1];
		Vec2_Physics A = simplex[2]; // make sure simlex[2] is always the newly added point and A is set to that point

		Vec2_Physics AB, AC, AO;
		AB = B - A;
		AC = C - A;
		AO = -A;

		Vec3 ABperp_temp = mathter::Cross(mathter::Cross(Vec3(AC.x, AC.y, 0.0f), Vec3(AB.x, AB.y, 0.0f)), Vec3(AB.x, AB.y, 0.0f));
		Vec3 ACperp_temp = mathter::Cross(mathter::Cross(Vec3(AB.x, AB.y, 0.0f), Vec3(AC.x, AC.y, 0.0f)), Vec3(AC.x, AC.y, 0.0f));

		Vec2_Physics ABperp = Vec2_Physics(ABperp_temp.x, ABperp_temp.y);
		Vec2_Physics ACperp = Vec2_Physics(ACperp_temp.x, ACperp_temp.y);

		if (mathter::Dot(ABperp, AO) > 0.0f)
		{
			direction = ABperp;
			simplex[0] = simplex[1];
			simplex[1] = simplex[2];
			simplex.pop_back();
			return false;
		}

		if (mathter::Dot(ACperp, AO) > 0.0f)
		{
			direction = ACperp;
			simplex[1] = simplex[2];
			simplex.pop_back();
			return false;
		}
		
		return true;
	}

	bool handleSimplex(std::vector<Vec2_Physics>& simplex, Vec2_Physics& direction)
	{
		if (simplex.size() == 2) return lineCase(simplex, direction);

		return triangleCase(simplex, direction);
	}

	void CollisionDetectionScene::CreateObjects(int& count)
	{
		m_PhysicsObjects.emplace_back(CreateRef<PhysicsObject_Square>(
			Vec2{-2.0f, 4.0f},
			Vec4{ 0.2f + count, 0.3f, 0.8f, 1.0f },
			count
		));

		// TODO: aabb collision detection
		m_PhysicsObjects[count]->SetMotionLimit(m_SceneWidth, m_SceneHeight);
		//m_PhysicsObjects.push_back(&m_CirclePhyicsObjectContainer[count]);
	}
	
	CollisionDetectionScene::CollisionDetectionScene(float height, float width, int numberOfObjects)
		:	m_SceneHeight(height), m_SceneWidth(width), m_NumberOfObjects(numberOfObjects)
	{
	}


	bool CollisionDetectionScene::FindCollisions(Ref<MGE::PhysicsObject_Square> Object_A, Ref<MGE::PhysicsObject_Square> Object_B, std::vector<Vec2_Physics>& Result_Convexhull)
	{
		std::vector<Vec2_Physics> Minkowski_sub;
		// std::vector<Vec2_Physics> Result_Convexhull;
		Minkowski_sub.reserve(Object_A->GetNumOfVertices() * Object_B->GetNumOfVertices());

		for (int i = 0; i < Object_A->GetNumOfVertices(); i++)
		{
			for (int j = 0; j < Object_B->GetNumOfVertices(); j++)
			{
				Minkowski_sub.push_back(Vec2_Physics{
					Object_A->GetVertices(i).x + Object_A->GetPosition().x - Object_B->GetVertices(j).x - Object_B->GetPosition().x, 
					Object_A->GetVertices(i).y + Object_A->GetPosition().y - Object_B->GetVertices(j).y - Object_B->GetPosition().y
				});
			}
		}

		Result_Convexhull = convex_hull(Minkowski_sub);

		Vec2_Physics direction = mathter::Normalize(Object_A->GetPosition() - Object_B->GetPosition());
		std::vector<Vec2_Physics> simplex;
		simplex.push_back(support(Result_Convexhull, direction));
		direction = -simplex[0];

		while (true)
		{
			Vec2_Physics A = support(Result_Convexhull, direction);
			if (mathter::Dot(A, direction) < 0)
			{
				return false;
			}
			simplex.push_back(A);
			if (handleSimplex(simplex, direction))
			{
				return true;
			}
		}
	}

	void CollisionDetectionScene::ResolveCollision(Ref<MGE::PhysicsObject_Square> i, Ref<MGE::PhysicsObject_Square> j, std::vector<Vec2_Physics>& Result_Convexhull)
	{
#if GJK
		Vec2_Physics origin = { 0.0f, 0.0f };
		int min_index = 0;
		float min_distace = 100.0f; 
		Vec2_Physics collision_normal = { 1.0f, 1.0f };

		for (int i = 0; i < Result_Convexhull.size(); i++)
		{
			Vec2_Physics r = Result_Convexhull[i];
			Vec2_Physics line = Result_Convexhull[(i + 1) % Result_Convexhull.size()] - Result_Convexhull[i];
			Vec2_Physics normal = mathter::Normalize(Vec2_Physics(-line.y, line.x));
			
			float distance = mathter::Dot(r, normal);
			if (distance < 0.0f)
			{
				distance = -distance;
				normal = -normal;
			}

			if (distance < min_distace)
			{
				min_distace = distance;
				collision_normal = normal;
			}
		}
		printf("min_distance: %f\n", min_distace);

		Vec2 hit_distance = min_distace * collision_normal;

		// 16.0f is to make the collision more stable
		i->SetCurrentPosition(i->GetPosition() - (hit_distance)/16.0f);
		j->SetCurrentPosition(j->GetPosition() + (hit_distance)/16.0f);


#else
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
#endif
	}

#if GJK
	
	void CollisionDetectionScene::OnUpdate(Timestep ts)
	{
		static int count = 0;
		static int frame = 0;
		if (m_PhysicsObjects.size() < m_NumberOfObjects && frame % 50 == 0)
		{
			CreateObjects(count);
			frame = 0;
			count++;
		}
		frame++;

		int sub_step_max = 8;
		for (int sub_step = 0; sub_step < sub_step_max; sub_step++)
		{
			for (int i = 0; i < m_PhysicsObjects.size() - 1; i++)
			{
				for (int j = i + 1; j < m_PhysicsObjects.size(); j++)
				{
					std::vector<Vec2_Physics> Result_Convexhull;
					if (FindCollisions(m_PhysicsObjects[i], m_PhysicsObjects[j], Result_Convexhull))
					{
						ResolveCollision(m_PhysicsObjects[i], m_PhysicsObjects[j], Result_Convexhull);
					}
				}
			}
		
			for (int i = 0; i < m_PhysicsObjects.size(); i++)
			{
				m_PhysicsObjects[i]->OnUpdate(ts / sub_step_max);
			}
		}

		for (int j = 0; j < m_PhysicsObjects.size(); j++)
		{
			m_PhysicsObjects[j]->DrawPhysicsObject();
		}
	}
#endif

#if SAT
	void CollisionDetectionScene::OnUpdate(Timestep ts)
	{
		BroadPhase();

		for (int i = 0; i < m_NumberOfObjects; i++)
		{
			m_PhysicsObjects[i]->OnUpdate(ts);
		}
		for (int i = 0; i < m_NumberOfObjects; i++)
		{
			m_PhysicsObjects[i]->DrawPhysicsObject();
		}
	}

	void CollisionDetectionScene::BroadPhase()
	{
		for (int i = 0; i < m_NumberOfObjects - 1; i++)
		{
			for (int j = i + 1; j < m_NumberOfObjects; j++)
			{
				Arbiter newArb(m_PhysicsObjects[i], m_PhysicsObjects[j]);
				CollisionKey key(m_PhysicsObjects[i], m_PhysicsObjects[j]);
				
				// if two object has contacts 
				if (newArb.numContacts > 0)
				{
					ArbIter iter = m_CollisionContainer.find(key);
					if (iter == m_CollisionContainer.end())
					{
						m_CollisionContainer.insert(ArbPair(key, newArb));
					}
					else
					{
						iter->second.Update(newArb.CollisionPoints, newArb.numContacts);
					}
				}
				else
				{
					m_CollisionContainer.erase(key);
				}
			}
		}
	}
#endif

}
