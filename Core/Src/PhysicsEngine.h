/*
 * PhysicsEngine.h
 *
 *  Created on: Jan 1, 2026
 *      Author: ebadrehman
 */

#ifndef PHYSICSENGINE_H_
#define PHYSICSENGINE_H_

class PhysicsEngine {
private:
    float velocity;       // Current speed (m/s)
    float last_velocity;  // Speed from previous check

public:
    // Constructor (Startup)
    PhysicsEngine() {
        velocity = 0.0f;
        last_velocity = 0.0f;
    }

    // The Main Math Function
    // Input: Acceleration (m/s^2), Time passed (seconds)
    // Output: New Velocity
    float update(float acceleration, float dt) {
        // Basic Integration: V = V0 + (a * t)
        // We add a "Deadzone" so tiny table vibrations don't count
        if (acceleration > -0.05 && acceleration < 0.05) {
            acceleration = 0;
        }

        velocity = last_velocity + (acceleration * dt);
        last_velocity = velocity;

        return velocity;
    }

    // Reset (for when you start a new set)
    void reset() {
        velocity = 0.0f;
        last_velocity = 0.0f;
    }
};

#endif /* PHYSICSENGINE_H_ */
