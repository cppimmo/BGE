/*******************************************************************************
 * @file   Actor.cpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Definition of Actor class.
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include "Engine/EngineStd.hpp"
#include "Actors/Actor.hpp"
#include "Actors/ActorComponent.hpp"

BGE::Actor::Actor(ActorID aID)
{
}

BGE::Actor::~Actor(void)
{
}

bool BGE::Actor::Init(tinyxml2::XMLElement *pData)
{
	return false;
}

void BGE::Actor::PostInit(void)
{
	for (auto &[key, value] : m_components)
	{

	}
}

void BGE::Actor::Destroy(void)
{
	m_components.clear();
}

void BGE::Actor::Update(float deltaTime)
{
	for (auto &[key, value] : m_components)
	{

	}
}

std::string BGE::Actor::ToXML(void)
{
	return std::string();
}

void BGE::Actor::AddComponent(StrongActorComponentPtr pComponent)
{
}
