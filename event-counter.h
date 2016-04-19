/*
	Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <default_gui_model.h>

class EventCounter : public DefaultGUIModel {
	
	Q_OBJECT
	
	public:
		EventCounter(void);
		virtual ~EventCounter(void);
	
		void execute(void);
		void createGUI(DefaultGUIModel::variable_t *, int);
		void customizeGUI(void);
	
	protected:
		virtual void update(DefaultGUIModel::update_flags_t);
	
	private:
		double window;
		double window_upper_bound;
		double threshold;
		double event_count;
		int counter;
		double dt;
		double systime;
		long long count;
		long long last_event_time;
		long long interval;
	
	private slots:
		void reset(void);
};
