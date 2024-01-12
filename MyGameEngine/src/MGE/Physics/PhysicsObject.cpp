#include "MGEpch.h"
#include "PhysicsObject.h"
#include <mutex>

std::mutex position_mtx;

namespace MGE {

	CirclePhyicsObject::CirclePhyicsObject(Vec2_Physics Position, int uID, Vec2_Physics Velocity)
		: m_Position(Position), m_uID(uID), m_Velocity(Velocity), m_LastPosition(Vec2_Physics{ Position.x, Position.y })
	{

	}

	CirclePhyicsObject::~CirclePhyicsObject()
	{
	}

	void CirclePhyicsObject::DrawPhysicsObject()
	{
		//DebugDraw();
		Renderer2D::DrawCircle({ m_Position.x, m_Position.y, 0.0f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f});
	}

	void CirclePhyicsObject::DebugDraw()
	{
		std::vector<Vec3> vertices;

		vertices.emplace_back(Vec3( 
			m_PressureAcceleration.x * 0.005f,
			m_PressureAcceleration.y * 0.005f,
			0.0f));
		vertices.emplace_back(Vec3(
			m_PressureAcceleration.x * 0.005f,
			m_PressureAcceleration.y * 0.005f + 0.02f,
			0.0f));
		vertices.emplace_back(Vec3(
			0.0f,
			+ 0.02f,
			0.0f));
		vertices.emplace_back(Vec3(
			0.0f,
			0.0f,
			0.0f));

		Renderer2D::DrawQuad(m_Position, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, vertices);
	}

	void CirclePhyicsObject::UpdateVelocity(Vec2_Physics deltaVelocity)
	{
		m_Velocity += deltaVelocity;
	}

	void CirclePhyicsObject::ChangeVelocity(Vec2_Physics newVelocity)
	{
		m_Velocity = newVelocity;
	}

	void CirclePhyicsObject::UpdatePosition(Vec2_Physics DeltaPosition)
	{
		m_Position += DeltaPosition;
	}

	void CirclePhyicsObject::OnUpdate(Timestep ts)
	{
		// if(mathter::Length(m_Velocity) < 0.1f) return; // object static, dont update at all
		
		Vec2_Physics FractionDirection = mathter::Length(m_Velocity) > 0.0f ? -mathter::Normalize(m_Velocity) : Vec2{0.0f, 0.0f};

		float FractionAccelaration =  2.0f; // fraction 

		UpdateVelocity(/*40.0f * Vec2_Physics{ 0.0f, -1.0f } * ts + */FractionDirection * ts + m_PressureAcceleration * ts);
		UpdatePosition(m_Velocity * ts);
		ApplyMotionLimit();
	}

	void CirclePhyicsObject::OnUpdate_Verlet(Timestep ts)
	{
		Vec2_Physics velocity = m_Position - m_LastPosition;
		m_LastPosition = m_Position;
		m_Position += velocity + (Vec2_Physics{0.0f, -1.0f} * 2.5f) * ts * ts + (m_PressureAcceleration ) * ts * ts;
		m_Velocity = m_Position - m_LastPosition;
		ApplyMotionLimit_Verlet();
	}

	void CirclePhyicsObject::ApplyMotionLimit()
	{
		// if(mathter::Length(m_Velocity) < 0.1f) return;
		
		if (m_Position.x > m_XLimit) {
			ChangeVelocity(Vec2_Physics{ -0.9f * m_Velocity.x, m_Velocity.y});
			UpdatePosition(Vec2_Physics{ m_XLimit - m_Position.x , 0.0f });
		}

		if (m_Position.x < -m_XLimit) {
			ChangeVelocity(Vec2_Physics{ -0.9f * m_Velocity.x, m_Velocity.y});
			UpdatePosition(Vec2_Physics{ -m_XLimit - m_Position.x , 0.0f });
		}

		// adjust the potion after the physics, then render
		if (m_Position.y < -m_YLimit) {
			ChangeVelocity(Vec2_Physics{ m_Velocity.x, 0.9f * (-m_Velocity.y) });
			UpdatePosition(Vec2_Physics{ 0.0f , -m_YLimit - m_Position.y });
		}

		if (m_Position.y > -m_YLimit) {
			ChangeVelocity(Vec2_Physics{ m_Velocity.x, 0.9f * (-m_Velocity.y) });
			UpdatePosition(Vec2_Physics{ 0.0f , m_YLimit - m_Position.y });
		}
	}

	void CirclePhyicsObject::ApplyMotionLimit_Verlet()
	{
		if (m_Position.x > m_XLimit) {
			m_Position.x = (m_XLimit + (m_XLimit - m_Position.x));
		}

		if (m_Position.x < -m_XLimit) {
			m_Position.x = (-m_XLimit + (-m_XLimit - m_Position.x));
		}

		if (m_Position.y < -m_YLimit) {
			m_Position.y = -m_YLimit + (-m_YLimit - m_Position.y);
		}

		if (m_Position.y > m_YLimit) {
			m_Position.y = m_YLimit + (m_YLimit - m_Position.y);
		}
	}

	void CirclePhyicsObject::SetMotionLimit(float xLimit, float yLimit)
	{
		m_XLimit = xLimit - 0.5f;
		m_YLimit = yLimit - 0.5f;
	}

	// Perimeter of Convex hull for a given set of points

	bool Compare_Vec2(Vec2_Physics a, Vec2_Physics b)
	{
		return a.x < b.x || (a.x == b.x && a.y < b.y);

	}

	// Cross product of two vectors OA and OB
	// returns positive for counter clockwise
	// turn and negative for clockwise turn
	float cross_product(Vec2_Physics O, Vec2_Physics A, Vec2_Physics B)
	{
		return (A.x - O.x) * (B.y - O.y)
			- (A.y - O.y) * (B.x - O.x);
	}

	// Returns a list of points on the convex hull
	// in counter-clockwise order
	std::vector<Vec2_Physics> convex_hull(std::vector<Vec2_Physics> A)
	{
		int n = A.size(), k = 0;

		if (n <= 3)
			return A;

		std::vector<Vec2_Physics> ans(2 * n);

		// Sort points lexicographically
		sort(A.begin(), A.end(), Compare_Vec2);

		// Build lower hull
		for (int i = 0; i < n; ++i) {

			// If the point at K-1 position is not a part
			// of hull as vector from ans[k-2] to ans[k-1]
			// and ans[k-2] to A[i] has a clockwise turn
			while (k >= 2
				&& cross_product(ans[k - 2],
					ans[k - 1], A[i]) <= 0)
				k--;
			ans[k++] = A[i];
		}

		// Build upper hull
		for (size_t i = n - 1, t = k + 1; i > 0; --i) {

			// If the point at K-1 position is not a part
			// of hull as vector from ans[k-2] to ans[k-1]
			// and ans[k-2] to A[i] has a clockwise turn
			while (k >= t
				&& cross_product(ans[k - 2],
					ans[k - 1], A[i - 1]) <= 0.0f)
				k--;
			ans[k++] = A[i - 1];
		}

		// Resize the array to desired size
		ans.resize(k - 1);

		return ans;
	}

}