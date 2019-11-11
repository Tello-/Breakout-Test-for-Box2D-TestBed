#ifndef BREAKOUTTEST_H
#define BREAKOUTTEST_H

#include <vector>

# define M_PI           3.14159265358979323846  /* pi */



class Enclosure
{
public:
	Enclosure(b2Vec2 size, b2World& world)
	{
		b2BodyDef bd;
		b2Body* body = world.CreateBody(&bd);
		body->SetUserData("Enclosure");
			b2EdgeShape shape;

		// Floor
		shape.Set(b2Vec2(-(size.x / 2.0f), -(size.y / 2.0f)), b2Vec2(size.x / 2.0f, -size.y / 2.0f));
		body->CreateFixture(&shape, 0.0f);

		// Left wall
		shape.Set(b2Vec2(-(size.x / 2.0f), -(size.y / 2.0f)), b2Vec2(-(size.x / 2.0f), size.y / 2.0f));
		body->CreateFixture(&shape, 0.0f);

		// Right wall
		shape.Set(b2Vec2(size.x / 2.0f, -(size.y / 2.0f)), b2Vec2(size.x / 2.0f, size.y / 2.0f));
		body->CreateFixture(&shape, 0.0f);

		// Roof
		shape.Set(b2Vec2(-(size.x / 2.0f), size.y / 2.0f), b2Vec2(size.x / 2.0f, size.y / 2.0f));
		body->CreateFixture(&shape, 0.0f);
	}
};

class BreakoutTest : public Test
{
public:
	BreakoutTest() : m_enclosure{m_worldSize, *m_world}
	{
		m_world->SetGravity(b2Vec2{ 0.f,0.f });

		m_ball = createBall(PTM_RATIO(10.0f), PTM_RATIO(b2Vec2{ 0.f,-300.0f }), 0.f);
		
		m_ball->ApplyLinearImpulseToCenter(b2Vec2{ 20.0f, 20.0f }, true);
		
		generateBricks(6, 10, PTM_RATIO(b2Vec2{ 100.0f, 30.0f }));

	}
		
	static Test* Create()
	{
		return new BreakoutTest;
	}

	

private:

	b2Body* createBall(float radius, b2Vec2 pos, float32 angle);
	b2Body* generateBricks(int cols, int rows, b2Vec2 brickSize);


	virtual void Step(Settings* settings) {
		Test::Step(settings);
		for (auto all : m_bricksToDelete)
		{
			all->GetWorld()->DestroyBody(all);
		}
		m_bricksToDelete.clear();
	}
	virtual void BeginContact(b2Contact* contact)  override;

	// 30 px / 1 meter
	float32 PTM_RATIO(float32 pixels) { return pixels / 30.0f; }
	b2Vec2 PTM_RATIO(b2Vec2 pixelVec) 
	{ 
		b2Vec2 tempVec;
		tempVec.x = PTM_RATIO(pixelVec.x);
		tempVec.y = PTM_RATIO(pixelVec.y);
		return tempVec;
	}

private:
	b2Body* m_ball;
	b2Vec2 m_worldSize{ PTM_RATIO(b2Vec2{600, 800}) };
	Enclosure m_enclosure;
	std::vector<b2Body*> m_bricks;
	std::vector<b2Body*> m_bricksToDelete;
	
};

#endif

inline b2Body* BreakoutTest::createBall(float radius, b2Vec2 pos, float32 angle)
{
	b2BodyDef myBodyDef;
	myBodyDef.bullet = true;
	myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	myBodyDef.position.Set(pos.x, pos.y); //set the starting position
	myBodyDef.angle = angle; //set the starting angle

	b2Body* dynamicBody = m_world->CreateBody(&myBodyDef);
	dynamicBody->SetUserData("Ball");
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
inline b2Body * BreakoutTest::generateBricks(int cols, int rows, b2Vec2 brickSize)
{	

	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.allowSleep = true;
	bd.awake = false;	

	//shape definition
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(brickSize.x / 2.0f, brickSize.y / 2.0f); 

	//fixture definition
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 1;

	
	for (int i = 0; i < rows; ++i) 
	{
		for (int j = 0; j < cols; ++j)
		{
			int k = j + (cols * i);
			bd.position.Set( ((brickSize.x/2) + brickSize.x * j) - PTM_RATIO(300.0f), (brickSize.y / 2)+ brickSize.y * i);
			m_bricks.push_back(m_world->CreateBody(&bd));
			m_bricks[k]->SetUserData("Brick");
			m_bricks[k]->CreateFixture(&myFixtureDef);
		}
		
	}
	return NULL;
}

inline void BreakoutTest::BeginContact(b2Contact * contact)
{
	if (contact->GetFixtureA()->GetBody()->GetUserData() == "Ball" 
		&& contact->GetFixtureB()->GetBody()->GetUserData() == "Brick")
	{
		m_bricksToDelete.push_back(contact->GetFixtureB()->GetBody());
	}
	else if (contact->GetFixtureA()->GetBody()->GetUserData() == "Brick"
		&& contact->GetFixtureB()->GetBody()->GetUserData() == "Ball")
	{
		m_bricksToDelete.push_back(contact->GetFixtureA()->GetBody());
	}
}