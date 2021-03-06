#pragma once

#include "Math/Transform.h"
#include "Graphics/Shape.h"

namespace hummus
{
	class Scene;

	class Actor
	{
	public:
		enum class eType
		{
			PLAYER,
			ENEMY,
			PROJECTILE,
			EPROJECTILE,
			LOCATOR,
			PICKUP
		};
	public:
		Actor() {}
		Actor(const Transform& transform, const Shape& shape) : m_transform{ transform }, m_shape{ shape } {}
		virtual ~Actor() {}

		virtual void Destroy();

		virtual eType GetType() = 0;

		virtual bool Load(const std::string& fileName);
		virtual void Load(std::istream& stream);

		virtual void Update(float dt);
		virtual void Draw(Core::Graphics& graphics);

		virtual void OnCollision(Actor* collision) {};

		float GetRadius();

		void SetScene(Scene* scene) { m_scene = scene; }
		Transform& GetTransform() { return m_transform; }
		Shape& GetShape() { return m_shape; }

		void SetDestroy(bool destroy = true) { m_destroy = destroy; }
		bool GetDestroy() { return m_destroy; }

		void AddChild(Actor* child);
		Actor* GetParent() { return m_parent; }

	protected:
		bool m_destroy{ false };
		Scene* m_scene{ nullptr };

		Transform m_transform;
		Shape m_shape;

		Actor* m_parent{ nullptr };
		std::vector<Actor*> m_children;
	};
}