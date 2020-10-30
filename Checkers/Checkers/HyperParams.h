#pragma once

#define THREAD_COUNT 12 // specific for each device
#define GAME_COUNT 2048 //Games per Generation
#define TEMPERATURE 1.75 //Temperature Parameter for move chosing
#define MAX_PLY 128 //Max PLY per game
#define ITER_COUNT 1024*1 //LookAhead for MCTS

#define BATCH_SIZE 1024
#define BATCH_COUNT 2048
#define LEARN_COUNT BATCH_SIZE * BATCH_COUNT

#define LEARNINGRATE 0.1f

#define TEST_COUNT 16

#define PAUSE_INTERVAL 900 //seconds
#define PAUSE_TIME 60

#define PLAY_PATH "Plays/Generation"
#define MODEL_PATH "Model/Generation"