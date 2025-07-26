// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef RIGIDBODY_H
#define RIGIDBODY_H


struct RigidBody {
    float damping = 1.f;
    float restitution;
    bool isKinematic;
};


#endif
