// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALTEST_API IDamageable
{
public:
	IDamageable() = default;
	virtual ~IDamageable() = default;

	virtual void ApplyDamage(float damage) = 0;
	virtual bool CanReceiveDamage() = 0;
	virtual void Destroy() = 0;
};
