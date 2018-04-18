#pragma once
#pragma managed
using namespace System;

public ref class ExecuteInEditor : Attribute
{

};

public ref class DisallowMultipleComponent : Attribute
{

};

[AttributeUsageAttribute(AttributeTargets::All, Inherited = false)]
public ref class HideInInspector : Attribute
{

};