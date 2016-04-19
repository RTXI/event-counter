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

#include <event-counter.h>
#include <main_window.h>
#include <iostream>

extern "C" Plugin::Object *createRTXIPlugin(void){
	return new EventCounter();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Window", "Window size in seconds for event_counting event", DefaultGUIModel::PARAMETER
		| DefaultGUIModel::DOUBLE, },
	{ "Interval", "Interval between events in seconds", DefaultGUIModel::PARAMETER
		| DefaultGUIModel::DOUBLE, },
	{ "Threshold", "Detection threshold in volts", DefaultGUIModel::PARAMETER
		| DefaultGUIModel::DOUBLE, },
	{ "Event Count", "Events detected in specified window", DefaultGUIModel::STATE, }, 
	{ "Input Signal", "Signal to detect events on", DefaultGUIModel::INPUT, },
	{ "TTL Output", "Output TTL when event is detected", DefaultGUIModel::OUTPUT, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

EventCounter::EventCounter(void) : DefaultGUIModel("Event Counter", ::vars, ::num_vars) {
	setWhatsThis("<p><b>EventCounter:</b><br>QWhatsThis description.</p>");
	DefaultGUIModel::createGUI(vars, num_vars);
	customizeGUI();
	update(INIT);
	refresh();
	QTimer::singleShot(0, this, SLOT(resizeMe()));
}

EventCounter::~EventCounter(void) { }

void EventCounter::execute(void)
{
	systime = count * dt;
	output(0) = 0;

	if(systime > window_upper_bound)
	{
		event_count = (int)counter;
		counter = 0;
		window_upper_bound += window;
	}
	else
	{
		if(input(0) > threshold)
			if(RT::OS::getTime() > last_event_time + interval)
			{
				counter++;
				output(0) = 5;
				last_event_time = RT::OS::getTime();
			}
	}

	count++;
	return;
}

void EventCounter::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			systime = 0;
			dt = RT::System::getInstance()->getPeriod() * 1e-9; // time in seconds
			count = 0;
			event_count = 0;
			counter = 0;
			window = 60; // s
			window_upper_bound = window; // s
			threshold = 0.001; // V
			interval = 0.1; // s
			last_event_time = 0;
			setParameter("Window", window);
			setParameter("Threshold", threshold);
			setParameter("Interval", interval);
			setState("Event Count", event_count);
			break;
	
		case MODIFY:
			window = getParameter("Window").toDouble(); // s
			threshold = getParameter("Threshold").toDouble(); // V
			interval = getParameter("Interval").toDouble() * 1e9; // ns
			reset();
			break;

		case UNPAUSE:
			break;

		case PAUSE:
			reset();
			break;

		case PERIOD:
			dt = RT::System::getInstance()->getPeriod() * 1e-9; // time in seconds
			break;
	
		default:
			break;
	}
}

void EventCounter::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	QGroupBox *button_group = new QGroupBox;
	QPushButton *reset_button = new QPushButton("Reset Counter");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(reset_button);
	QObject::connect(reset_button, SIGNAL(clicked()), this, SLOT(reset()));
	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}

void EventCounter::reset(void)
{
	count = 0;
	counter = 0;
	systime = 0;
	last_event_time = 0;
	event_count = 0;
	window_upper_bound = window; // s
}

