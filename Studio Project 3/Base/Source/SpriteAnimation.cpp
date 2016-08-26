#include "SpriteAnimation.h"
#include "GL\glew.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col) :
Mesh(meshName),
m_row(row),
m_col(col),
m_currentTime(0),
m_currentFrame(0),
m_playCount(0)
{
    m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
    if (m_anim)
    {
        delete m_anim;
    }
}

void SpriteAnimation::Update(double dt)
{
    if (m_anim->animActive == true) // if animation happens
    {
        // Get Animation current time, m_currentTime
        // Animation current time increments with dt
        m_currentTime += static_cast<float>(dt);

        // Find int numFrame
        int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);

        // Get float frameTime
        // FrameTime = animTime / numFrame
        float frameTime = m_anim->animTime / numFrame;

        // Get animation current frame, m_currentFrame
        m_currentFrame = Math::Min(m_anim->endFrame,
            m_anim->startFrame + static_cast<int>(m_currentTime / frameTime));

        // Check if m_currentTime is greater than or equal to animTime 
        if (m_currentTime >= m_anim->animTime)
        {
            // Set animation end flag, ended to true
            m_anim->ended = true;

            // Check if repeatCount == 0
            if (m_anim->repeatCount == 0)
            {
                // Set animActive = false
                m_anim->animActive = false;
                // reset m_currentTime
                m_currentTime = 0.f;
                // set m_currentFrame = startFrame
                m_currentFrame = m_anim->startFrame;
            }

            // Check if repeatCount == 1
            if (m_anim->repeatCount == 1)
            {
                // reset m_currentTime
                m_currentTime = 0.f;
                // set m_currentFrame = startFrame
                m_currentFrame = m_anim->startFrame;
            }
        }
  /*      Billboard(SharedData::GetInstance()->PlayerPos);
        if (m_anim->moveable == true)
        {
            ChasePlayer(dt, SharedData::GetInstance()->PlayerPos);
        }*/
    }
}

void SpriteAnimation::Billboard(Vector3 Player)
{
    Vector3 initView(0, 0, 1);
    Vector3 wantView(Player - this->SpritePos);
    Vector3 normal(0, 1, 0);

    if (this->SpritePos != (0, 0, 0))
    {
        wantView.Normalize();
    }

    this->SpriteAngle = Math::RadianToDegree(acos(initView.Dot(wantView)));
    Vector3 Crossed = initView.Cross(wantView);
    if (Crossed.Dot(normal) < 0)
    {
        this->SpriteAngle *= -1;
    }
}

void SpriteAnimation::ChasePlayer(double dt, Vector3 Player)
{
    Vector3 DirVec = Player - SpritePos;
    if (Player != SpritePos)
    {
        DirVec.Normalize();
    }

    if ((sqrt((pow(SpritePos.x - Player.x, 2)) + (pow(SpritePos.z - Player.z, 2)))) > 50)
    {
        SpritePos.x += DirVec.x * 100.f * dt;
        SpritePos.z += DirVec.z * 100.f * dt;
    }
}

void SpriteAnimation::Render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    if (mode == DRAW_LINES)
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
    else if (mode == DRAW_TRIANGLE_STRIP)
        glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
    else
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisableVertexAttribArray(3);
}