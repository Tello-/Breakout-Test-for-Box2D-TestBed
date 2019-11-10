#ifndef FOOTEST_H
#define FOOTEST_H


# define M_PI           3.14159265358979323846  /* pi */

class BreakoutTest : public Test
{
public:
	BreakoutTest()
	{
		m_world->SetGravity(b2Vec2{ 0.f,0.f });

		createEnclosure(m_worldSize);

		m_ball = createBall(PTM_RATIO(30.f), b2Vec2{ 0.f,0.f }, 0.f);
		
		m_ball->ApplyLinearImpulseToCenter(b2Vec2{ 20.0f, 20.0f }, true);
	
	}

	
	static Test* Create()
	{
		return new BreakoutTest;
	}

private:
	void createEnclosure(b2Vec2 size);
	void createWall(b2Vec2 size, b2Vec2 pos, float32 angle);
	b2Body* createBall(float radius, b2Vec2 pos, float32 angle);	

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
	b2Body* m_ball;
	b2Vec2 m_worldSize{ PTM_RATIO(b2Vec2{600, 800}) };
};

#endif

void BreakoutTest::createEnclosure(b2Vec2 size)
{
	b2BodyDef bd;
	b2Body* ground = m_world->CreateBody(&bd);

	b2EdgeShape shape;

	// Floor
	shape.Set(b2Vec2(-size.x, -size.y), b2Vec2(size.x, -size.y));
	ground->CreateFixture(&shape, 0.0f);

	// Left wall
	shape.Set(b2Vec2(-size.x, -size.y), b2Vec2(-size.x, size.y));
	ground->CreateFixture(&shape, 0.0f);

	// Right wall
	shape.Set(b2Vec2(size.x, -size.y), b2Vec2(size.x, size.y));
	ground->CreateFixture(&shape, 0.0f);

	// Roof
	shape.Set(b2Vec2(-size.x, size.y), b2Vec2(size.x, size.y));
	ground->CreateFixture(&shape, 0.0f);
}
void BreakoutTest::createWall(b2Vec2 size, b2Vec2 pos, float32 angle)
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
b2Body* BreakoutTest::createBall(float radius, b2Vec2 pos, float32 angle)
{
	b2BodyDef myBodyDef;
	myBodyDef.bullet = true;
	myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	myBodyDef.position.Set(pos.x, pos.y); //set the starting position
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
	return dynamicBody;
}
