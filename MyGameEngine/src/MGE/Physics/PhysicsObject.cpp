#include "MGEpch.h"
#include "PhysicsObject.h"

namespace MGE {

	CirclePhyicsObject::CirclePhyicsObject(Vec2 Position, Vec4 Color, int uID)
		: m_Position(Position), m_Color(Color), m_uID(uID)
	{

	}

	CirclePhyicsObject::~CirclePhyicsObject()
	{
	}

	void CirclePhyicsObject::DrawPhysicsObject()
	{
		Renderer2D::DrawCircle({ m_Position.x, m_Position.y, 0.0f }, m_Color);
	}

	void CirclePhyicsObject::UpdateVelocity(Vec2 deltaVelocity)
	{
		m_Velocity += deltaVelocity;
	}

	void CirclePhyicsObject::ChangeVelocity(Vec2 newVelocity)
	{
		m_Velocity = newVelocity;
	}

	void CirclePhyicsObject::UpdatePosition(Vec2 DeltaPosition)
	{
		// DeltaPosition = on_Ground ? Vec2{ DeltaPosition.x, 0.0f } : DeltaPosition;
		m_Position += DeltaPosition;
	}

	void CirclePhyicsObject::OnUpdate(Timestep ts)
	{
		on_Ground = m_Position.y < -m_YLimit;
		// m_Position.y = on_Ground ? -m_YLimit : m_Position.y;

		m_Gravity = on_Ground ? 0.0f : 9.8f;
		// m_Velocity = on_Ground ? Vec2{ m_Velocity.x, 0.0f } : m_Velocity;

		Vec2 FractionDirection = mathter::Length(m_Velocity) > 0.0f ? -mathter::Normalize(m_Velocity) : Vec2{0.0f, 0.0f};

		float FractionAccelaration =  2.0f; // fraction 

		if (std::abs(m_Position.x) > m_XLimit) {
			ChangeVelocity(Vec2{ -0.9f * m_Velocity.x, m_Velocity.y });
		}

		if (m_Position.y < -m_YLimit) {
			ChangeVelocity(Vec2{ m_Velocity.x, 0.9f * (-m_Velocity.y) });
			UpdatePosition(Vec2{ 0.0f , -m_YLimit - m_Position.y });
		}
		
		UpdateVelocity(m_Gravity * Vec2{ 0.0f, -1.0f } * ts + FractionDirection * FractionAccelaration * ts);

		UpdatePosition(m_Velocity * ts);
	}

	void CirclePhyicsObject::SetMotionLimit(float xLimit, float yLimit)
	{
		m_XLimit = xLimit;
		m_YLimit = yLimit;
	}

	// Perimeter of Convex hull for a given set of points

	bool Compare_Vec2(Vec2 a, Vec2 b) 
	{
		return a.x < b.x || (a.x == b.x && a.y < b.y);

	}

	// Cross product of two vectors OA and OB
	// returns positive for counter clockwise
	// turn and negative for clockwise turn
	float cross_product(Vec2 O, Vec2 A, Vec2 B)
	{
		return (A.x - O.x) * (B.y - O.y)
			- (A.y - O.y) * (B.x - O.x);
	}

	// Returns a list of points on the convex hull
	// in counter-clockwise order
	std::vector<Vec2> convex_hull(std::vector<Vec2> A)
	{
		int n = A.size(), k = 0;

		if (n <= 3)
			return A;

		std::vector<Vec2> ans;
		ans.reserve(2 * n);
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