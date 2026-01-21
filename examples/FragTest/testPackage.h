#pragma once

#include <frag/package/IPackage.h>

class TestPackage : public frag::IPackage {
public:
    void setupPackage() override {
        rPackageName("TestPackage");

        // Register Game States
        rGameState("MainMenuState");
        rGameState("GameplayState");
        rGameState("PauseMenuState");

        // Register Assets
        rAsset(0, "PlayerModel", "assets/models/player.obj");
        rAsset(1, "EnemyTexture", "assets/textures/enemy.png");

        // Register Components
        // rComponent("HealthComponent", HealthComponentClass);
        // rComponent("MovementComponent", MovementComponentClass);

        // Register Scenes
        // rScene("Level1Scene", Level1SceneClass);
        // rScene("Level2Scene", Level2SceneClass);

        finishPackage();
    }
};