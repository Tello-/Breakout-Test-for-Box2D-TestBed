#ifndef FOOTEST_H
#define FOOTEST_H


# define M_PI           3.14159265358979323846  /* pi */

class FooTest : public Test
{
public:
	FooTest()
	{
		m_world->SetGravity(b2Vec2{ 0.f,0.f });

		createWall(b2Vec2{ m_worldSize.x, 1.f }, b2Vec2{ 0.0f, -m_worldSize.y }, 0.0f); // floor (s/p/a)
		createWall(b2Vec2{ 1.f, m_worldSize.y }, b2Vec2{ -m_worldSize.x, 0.0f }, M_PI); // left wall
		createWall(b2Vec2{ 1.f, m_worldSize.y }, b2Vec2{ m_worldSize.x, 0.0f }, M_PI); // right wall
		createWall(b2Vec2{ m_worldSize.x , 1.f }, b2Vec2{ 0.0f, m_worldSize.y }, 0.0f); // top wall

		createBall(PTM_RATIO(30.f), b2Vec2{ 0.f,0.f }, 0.f);
	}

	void Step(Settings* settings)
	{
		//run the default physics and rendering
		Test::Step(settings);

		//show some text in the main screen
		g_debugDraw.DrawString(5, m_textLine, "Now we have a foo test");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new FooTest;
	}

private:

	void createWall(b2Vec2 size, b2Vec2 pos, float32 angle)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(pos.x, pos.y);
		bodyDef.angle = angle;

		b2Body* body = m_world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(size.x, size.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.f;
		body->CreateFixture(&fixtureDef);
	}

	void createBall(float32 radius, b2Vec2 pos, float32 angle)
	{
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
		myBodyDef.position.Set(pos.x,pos.y); //set the starting position
		myBodyDef.angle = angle; //set the starting angle

		b2Body* dynamicBody = m_world->CreateBody(&myBodyDef);

		b2CircleShape circShape;
		circShape.m_radius = radius;

		b2FixtureDef circFixtureDef;

		circFixtureDef.shape = &circShape;
		circFixtureDef.density = 0.f;
		circFixtureDef.restitution = 1.f;
		circFixtureDef.friction = 0.f;
		dynamicBody->CreateFixture(&circFixtureDef);
	}

	// 30 px / 1 meter
	float32 PTM_RATIO(float32 pixels) { return pixels * 0.0333f; }
	b2Vec2 PTM_RATIO(b2Vec2 pixelVec) 
	{ 
		b2Vec2 tempVec;
		tempVec.x = pixelVec.x * 0.0333f;
		tempVec.y = pixelVec.y * 0.0333f;
		return tempVec;
	}

private:
	b2Vec2 m_worldSize{ PTM_RATIO(b2Vec2{600.f, 800.f}) };
};

#endif