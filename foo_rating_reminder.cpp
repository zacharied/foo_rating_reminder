#include "stdafx.h"
#include <string>

#define REMIND_TIME 30

DECLARE_COMPONENT_VERSION(
	"foo_rating_reminder",
	"0.0.1",
	"Reminds you to rate a track when it ends.\n"
);

class rating_reminder : public play_callback_impl_base {
private:
	bool sent_reminder;
	metadb_handle_ptr track;

public:
	rating_reminder() : play_callback_impl_base(flag_on_playback_time | flag_on_playback_new_track) { }

	virtual void on_playback_new_track(metadb_handle_ptr p_track) {
		track = p_track;
		sent_reminder = false;
	}

	virtual void on_playback_time(double p_time) {
		if (track->get_length() - p_time < REMIND_TIME && !sent_reminder) {
			sent_reminder = true;

			// TODO Popup window for reminder.
			// This is a placeholder.
			FB2K_console_formatter() << "Rate the currently playing track!";
		}
	}
};

class rating_reminder_service : public initquit {
private:
	rating_reminder *r;

public:
	virtual void on_init() {
		r = new rating_reminder();
	}

	virtual void on_quit() {
		delete r;
	}
};

// Register the service with foobar2k.
initquit_factory_t<rating_reminder_service> g_init;