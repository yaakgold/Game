#include "Player.h"
#include "Projectile.h"
#include "Object/Scene.h"
#include "../Game.h"
#include "Graphics/ParticleSystem.h"
#include <fstream>
#include <Math\MathFile.h>


bool Player::Load(const std::string& fileName)
{
    bool success = false;

    std::ifstream stream(fileName);
    if (stream.is_open())
    {
        success = true;

        //Load Actor
        Actor::Load(stream);

        //Load thrust
        std::string line;
        std::getline(stream, line);
        thrust = std::stof(line);

        //Load Rotation Rate
        std::getline(stream, line);
        rotSpeed = std::stof(line);

        stream.close();
    }

    return success;
}

void Player::Update(float dt)
{
    fireTimer += dt;

    if (Core::Input::IsPressed(VK_SPACE) && fireTimer >= fireRate)
    {
        fireTimer = 0;
        Projectile* proj = new Projectile;
        proj->Load("projectile.txt");
        proj->GetTransform().position = m_transform.position;
        proj->GetTransform().angle = m_transform.angle;
        proj->SetThrust(100);
        m_scene->AddActor(proj);
    }


    Vector2 force;

    if (Core::Input::IsPressed(Core::Input::KEY_UP) || Core::Input::IsPressed('W')) { force = Vector2::forward * thrust; }
    if (Core::Input::IsPressed(Core::Input::KEY_DOWN) || Core::Input::IsPressed('S')) { force = Vector2::forward * -thrust; }
    force = Vector2::Rotate(force, m_transform.angle);

    //Gravity
    //force += Vector2{ 0, 200 };

    velocity += force * dt;
    velocity *= .99f;

    m_transform.position += velocity * dt;

    //Rotate
    if (Core::Input::IsPressed(Core::Input::KEY_LEFT) || Core::Input::IsPressed('A')) { m_transform.angle -= dt * rotSpeed; }
    if (Core::Input::IsPressed(Core::Input::KEY_RIGHT) || Core::Input::IsPressed('D')) { m_transform.angle += dt * rotSpeed; }

    //Screen wrapping
    if (m_transform.position.x > 800) m_transform.position.x = 0;
    if (m_transform.position.x < 0) m_transform.position.x = 800;

    if (m_transform.position.y < 0) m_transform.position.y = 600;
    if (m_transform.position.y > 600) m_transform.position.y = 0;

    if (force.LengthSqr() > 0)
    {
        g_particleSystem.Create(m_transform.position, m_transform.angle + PI, 20, 1, Color{ 1, 1, 0 }, 1, 100, 200);
    }

    m_transform.Update();

}

void Player::OnCollision(Actor* collision)
{
    if (collision->GetType() == eType::ENEMY)
    {
        m_scene->GetGame()->SetState(Game::eState::GAME_OVER);
    }
}