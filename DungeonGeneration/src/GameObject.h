#pragma once

class GameObject
{
public:
    GameObject();
    virtual ~GameObject() = default;

    virtual void Draw();

    bool GetIsActive() const {return IsActive;}
    void SetIsActive(bool active) {IsActive = active;}

private:

    bool IsActive;
};
