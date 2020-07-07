#pragma once
#include "DiceConsole.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include "DiceSchedule.h"
#include "StrExtern.hpp"
#include "ManagerSystem.h"
#include "DiceCloud.h"
#pragma warning(disable:28159)

inline time_t tNow = time(NULL);

int sendSelf(const string msg);

void cq_exit(DiceJob& job);
void cq_restart(DiceJob& job);

void auto_save(DiceJob& job);
void check_system(DiceJob& job);

void clear_image(DiceJob& job);

void clear_group(DiceJob& job);

void cloud_beat(DiceJob& job);
void dice_update(DiceJob& job);
