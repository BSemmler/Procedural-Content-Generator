#pragma once
#include "pch.h"

namespace KGV::System
{
	class Application
	{
	public:
		Application();
		~Application();

		virtual bool init() = 0;
		virtual void shutdown() = 0;
		virtual int run() = 0;

	protected:

	};
}