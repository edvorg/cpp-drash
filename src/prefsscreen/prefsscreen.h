#ifndef PREFSSCREEN_H
#define PREFSSCREEN_H

#include "../root/screen.h"

namespace drash
{

		class CPrefsScreen : public CScreen
		{
		public:
				CPrefsScreen() = delete;
				explicit CPrefsScreen(CRoot & _root);
				virtual ~CPrefsScreen();

				virtual void Step(double _dt) override;
				virtual void Render() override;

		protected:
		private:
		};

} // namespace drash

#endif
