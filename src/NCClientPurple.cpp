/*
 * NCClientPurple.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: dwkimm01
 */

#include "NCClientPurple.h"

namespace ncpp
{
namespace ncclientpurple
{

NCClientPurple::NCClientPurple
	( const NCClientPurple::String &name
	, const NCClientPurple::String &password
	, const String &protocol
	, std::function<void(const String&, const int, const int)> connectionStepCB
	, std::function<void(const String&, const String&)> msgReceivedCB
	, std::function<void(const String&, const String&)> debugLogCB
	, std::function<void(const String&)> buddySignedOnCB
	)
{}


NCClientPurple::~NCClientPurple() {}


NCClientPurple::String NCClientPurple::getName() {}

// --------------------------------------------------------------
// State change
void NCClientPurple::connect() {}
void NCClientPurple::disconnect() {}

// --------------------------------------------------------------
// Commands
void NCClientPurple::sendTyping(const String &who, const String &msg, bool done) {}
void NCClientPurple::msgSend(const String &who, const String &msg) {}
void NCClientPurple::questionAnswerBool(const int id, const bool response) {}
void NCClientPurple::addBuddy(const NCClientPurple::String &who, const NCClientPurple::String &group) {}
void NCClientPurple::removeBuddy(const NCClientPurple::String &who, const NCClientPurple::String &group) {}
void NCClientPurple::setAway(const NCClientPurple::String &state, const NCClientPurple::String &msg) {}


// --------------------------------------------------------------
// Callbacks
// Connection status
void NCClientPurple::connectionProgress(const String &text, const int step, const int count) {}
void NCClientPurple::connected() {}
void NCClientPurple::disconnected() {}
void NCClientPurple::connectionNotice(const String &text) {}
void NCClientPurple::disconnectNotice(const String &text, const int reason) {}
void NCClientPurple::contactTyping() {}

void NCClientPurple::msgReceived(const String &who, const String &msg) {}

void NCClientPurple::questionAskedBool(const int id, const String &question) {}

void NCClientPurple::debugLog(const String &level, const String &logLine) {}


}} 


#if 0
#include <signal.h>
#include <libpurple/purple.h>
#include <thread>
#include "NCClientPurple.h"
#include "NCStringUtils.h"
#include "NCEnvironmentVariables.h"
#include "NCUtils.h"

namespace ncpp
{
namespace ncclientpurple
{

// TODO, would be nice to make these not static
PurpleConnectionUiOps connection_uiops;
PurpleRequestUiOps bee_request_uiops;



// -----------------------------------------------------------------
class NCClientPurple::Data
{
private:
	std::function<void(const String&, const String&)> p_msgReceivedCB;

public:
	std::function<void(const String&, const int, const int)> p_connectionStepCB;
	std::function<void(const String&, const String&)> p_debugLogCB;
        std::function<void(const String&)> p_buddySignedOnCB;
private:

	std::shared_ptr<std::thread> loopThread;
	GMainLoop* loop; //  = g_main_loop_new(NULL, FALSE);

public:
	std::shared_ptr<PurpleAccount> purpleAccount;


	Data
		( std::function<void(const String&, const int, const int)> connectionStepCB
		, std::function<void(const String&, const String&)> msgReceivedCB
		, std::function<void(const String&, const String&)> debugLogCB 
                , std::function<void(const String&)> buddySignedOn )
		: p_msgReceivedCB(msgReceivedCB)
		, p_connectionStepCB(connectionStepCB)
		, p_debugLogCB(debugLogCB)
                , p_buddySignedOnCB(buddySignedOn)
		, loopThread(0)
		, loop(0)
		, purpleAccount(0)
	{

		core_uiops =
		{
			NULL,
			NULL,
			ui_init,
			NULL,
			// padding
			NULL,
			NULL,
			NULL,
			NULL
		};


		connection_uiops =
		{
			connect_progress,          // connect_progress
			NULL,                      // connected
			NULL,                      // disconnected
			notice,                    // notice
			NULL,                      // report_disconnect
			NULL,                      // network_connected
			network_disconnected,      // network_disconnected
			report_disconnect_reason,  // report_disconnect_reason
			NULL,
			NULL,
			NULL
		};


		bee_request_uiops =
		{
			NULL,
			NULL,
			prplcb_request_action,
			NULL,
			NULL,
			NULL,
			NULL,
		};

		glib_eventloops =
		{
			g_timeout_add,
			g_source_remove,
			glib_input_add,
			g_source_remove,
			NULL,
		#if GLIB_CHECK_VERSION(2,14,0)
			g_timeout_add_seconds,
		#else
			NULL,
		#endif
			// padding
			NULL,
			NULL,
			NULL
		};


	}


	void start()
	{
		// TODO, probably need to free this with some gmain_loop_delete or something
		//GMainLoop *loop = g_main_loop_new(NULL, FALSE);
		loop = g_main_loop_new(NULL, FALSE);

		loopThread.reset( new std::thread(runThread, loop));
	}

	static void runThread(GMainLoop *l)
	{
		g_main_loop_run(l);
	}


	PurpleCoreUiOps core_uiops;
	PurpleEventLoopUiOps glib_eventloops;


	static void ui_init ()
	{
		purple_connections_set_ui_ops(&connection_uiops);
		purple_request_set_ui_ops( &bee_request_uiops );
	}

	static void connect_progress
		( PurpleConnection *gc
		, const char *text
		, size_t step
		, size_t step_count )
	{
	}

	static void notice(PurpleConnection *gc, const char *text)
	{
	}

	static void network_disconnected(void)
	{
	}

	static void report_disconnect_reason(PurpleConnection *gc, PurpleConnectionError reason, const char *text)
	{
		// PurpleAccount *account = purple_connection_get_account(gc);
		// char* name = purple_account_get_username(account);
		// char* protocol = purple_account_get_protocol_id(account);
	}

	// Request action data
	struct prplcb_request_action_data
	{
		void *user_data, *bee_data;
		PurpleRequestActionCb yes, no;
		int yes_i, no_i;
	};

	static void prplcb_request_action_yes( void *data )
	{
		struct prplcb_request_action_data *pqad = static_cast<prplcb_request_action_data*>(data);

		if( pqad->yes )
		{
			printf("xxx yes\n");
			pqad->yes( pqad->user_data, pqad->yes_i );
		}

		g_free( pqad );
	}



	static void *prplcb_request_action
		( const char *title
		, const char *primary
		, const char *secondary
		, int default_action
		, PurpleAccount *account
		, const char *who
		, PurpleConversation *conv
		, void *user_data
		, size_t action_count
		, va_list actions )
	{

		struct prplcb_request_action_data *pqad;
		char *q;
		pqad = g_new0( struct prplcb_request_action_data, 1 );

		for( size_t i = 0; i < action_count; i ++ )
		{
			char *caption;
			void *fn;

			caption = va_arg( actions, char* );
			fn = va_arg( actions, void* );

			if( strstr( caption, "Accept" ) || strstr( caption, "OK" ) )
			{
				pqad->yes = reinterpret_cast<PurpleRequestActionCb>(fn);
				pqad->yes_i = i;
			}
			else if( strstr( caption, "Reject" ) || strstr( caption, "Cancel" ) )
			{
				pqad->no = reinterpret_cast<PurpleRequestActionCb>(fn);
				pqad->no_i = i;
			}
		}

		pqad->user_data = user_data;
		prplcb_request_action_yes( pqad );

		// TODO: IRC stuff here :-(
		q = g_strdup_printf( "Request: %s\n\n%s\n\n%s", title, primary, secondary );
		printf("xxx (%s) (%s) (%s)\n", title, primary, secondary );
	    // pqad->bee_data = query_add( local_bee->ui_data, purple_ic_by_pa( account ), q,
	    // prplcb_request_action_yes, prplcb_request_action_no, g_free, pqad );
		g_free( q );
		return pqad;
	}


	typedef struct _PurpleGLibIOClosure
	{
	   PurpleInputFunction function;
	   guint result;
	   gpointer data;
	} PurpleGLibIOClosure;


	static void purple_glib_io_destroy(gpointer data)
	{
		g_free(data);
	}

#define PURPLE_GLIB_READ_COND  (G_IO_IN | G_IO_HUP | G_IO_ERR)
#define PURPLE_GLIB_WRITE_COND (G_IO_OUT | G_IO_HUP | G_IO_ERR | G_IO_NVAL)


	static gboolean purple_glib_io_invoke(GIOChannel *source, GIOCondition condition, gpointer data)
	{
		PurpleGLibIOClosure *closure = static_cast<PurpleGLibIOClosure*>(data);
		PurpleInputCondition purple_cond = static_cast<PurpleInputCondition>(0);

		if (condition & PURPLE_GLIB_READ_COND)
			purple_cond = static_cast<PurpleInputCondition>(purple_cond | PURPLE_INPUT_READ);
		if (condition & PURPLE_GLIB_WRITE_COND)
			purple_cond = static_cast<PurpleInputCondition>(purple_cond | PURPLE_INPUT_WRITE);

		closure->function(closure->data, g_io_channel_unix_get_fd(source),
				  purple_cond);

		return TRUE;
	}




	static guint glib_input_add
		( gint fd
		, PurpleInputCondition condition
		, PurpleInputFunction function
		, gpointer data )
	{
		PurpleGLibIOClosure *closure = g_new0(PurpleGLibIOClosure, 1);
		GIOChannel *channel;
		GIOCondition cond = static_cast<GIOCondition>(0);

		closure->function = function;
		closure->data = data;

		if (condition & PURPLE_INPUT_READ)
			cond = static_cast<GIOCondition>(cond | PURPLE_GLIB_READ_COND);
		if (condition & PURPLE_INPUT_WRITE)
			cond = static_cast<GIOCondition>(cond | PURPLE_GLIB_WRITE_COND);

		channel = g_io_channel_unix_new(fd);
		closure->result = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, cond,
						      purple_glib_io_invoke, closure, purple_glib_io_destroy);

		g_io_channel_unref(channel);
		return closure->result;
	}


	static void received_im_msg
		( PurpleAccount *account
		, char *sender
		, char *message
		, PurpleConversation *conv
		, PurpleMessageFlags flags
		, void* data
		)
	{
		// TODO, do we still need this?
		if (conv==NULL)
		{
			conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, account, sender);
		}

		// Get context
		Data* t = static_cast<Data*>(data);
		if(t)
		{
			const std::string from = ncstringutils::NCStringUtils::nullIsEmpty(sender);
			const std::string msg = ncstringutils::NCStringUtils::nullIsEmpty(message);
			// Send to callback
			t->p_msgReceivedCB(from, msg);
		}
	}

	static void signed_on(PurpleConnection *gc, void* data)
	{
//		printf("Account connected: \"%s\" (%s)\n", purple_account_get_username(account), purple_account_get_protocol_id(account));
		Data* t = static_cast<Data*>(data);
		if(t)
		{
			PurpleAccount *account = purple_connection_get_account(gc);
			const std::string username = ncstringutils::NCStringUtils::nullIsEmpty(purple_account_get_username(account));
			const std::string protocol = ncstringutils::NCStringUtils::nullIsEmpty(purple_account_get_protocol_id(account));

			t->p_debugLogCB("INFO", "Account connected: " + username + ", " + protocol);
		}


	}


	// Send a message
	static int send_msg(PurpleAccount *account, const char *name, const char *msg)
	{
		PurplePlugin* prpl = NULL;
		PurplePluginProtocolInfo* prpl_info = NULL;
		// PurpleConversation* conv;

		prpl = purple_connection_get_prpl(account->gc);
		if (prpl == NULL)
			return -1;

		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

		PurpleMessageFlags flags = static_cast<PurpleMessageFlags>(0);
		if (prpl_info->send_im)
		{
			prpl_info->send_im(account->gc, name, msg, flags);
		}
		return 0;
	}


        static void buddy_signed_on(PurpleBuddy *buddy, void* data)
        {
            Data* t = static_cast<Data*>(data);
	    // printf("Buddy \"%s\" (%s) signed on\n", purple_buddy_get_name(buddy), purple_account_get_protocol_id(purple_buddy_get_account(buddy)));
            if(t)
            {
               t->p_buddySignedOnCB(purple_buddy_get_name(buddy));
            } 
        }

};

// -----------------------------------------------------------------
NCClientPurple::NCClientPurple
	( const NCClientPurple::String &name
	, const String &password
	, const String &protocol
	, std::function<void(const String&, const int, const int)> connectionStepCB
	, std::function<void(const String&, const String&)> msgReceivedCB
	, std::function<void(const String&, const String&)> debugLogCB
   	, std::function<void(const String&)> buddySignedOnCB 
   	)
	: p_name(name)
	, p_password(password)
	, p_protocol(protocol)
	, p_connectionStepCB(connectionStepCB)
	, p_msgReceivedCB(msgReceivedCB)
	, p_debugLogCB(debugLogCB)
	, p_data(new Data(connectionStepCB, p_msgReceivedCB, debugLogCB, buddySignedOnCB) )
{
	// First check to see if prpl-sipe is being used without
	// having set "export NSS_SSL_CBC_RANDOM_IV=0"
	if( "prpl-sipe" == p_protocol
		&& "0" != ncenvironmentvariables::NCEnvironmentVariables::getEnv("NSS_SSL_CBC_RANDOM_IV"))
	{
		p_debugLogCB("DEBUG", "Warning, prpl-sipe used without NSS_SSL_CBC_RANDOM_IV = 0, setting it");
		ncenvironmentvariables::NCEnvironmentVariables::setEnv("NSS_SSL_CBC_RANDOM_IV", "0");
	}

	signal(SIGCHLD, SIG_IGN);

	// -------------------------------------------------------------
	// init_libpurple
	// TODO maybe make these settable
	// Settings
	const std::string CUSTOM_USER_DIRECTORY = "/dev/null";
	const std::string CUSTOM_PLUGIN_PATH = "";
	const std::string PLUGIN_SAVE_PREF = "/purple/user/plugins/saved";
	const std::string UI_ID = "user";

	// Set a custom user directory (optional)
	purple_util_set_user_dir(CUSTOM_USER_DIRECTORY.c_str());

	// We do not want any debugging for now to keep the noise to a minimum
	purple_debug_set_enabled(FALSE);

	// Set the core-uiops, which is used to
	// 	- initialize the ui specific preferences.
	// 	- initialize the debug ui.
	// 	- initialize the ui components for all the modules.
	// 	- uninitialize the ui components for all the modules when the core terminates.
	purple_core_set_ui_ops(&p_data->core_uiops);

	// Set the uiops for the eventloop. If your client is glib-based, you can safely
	// copy this verbatim.
	// TODO, can i get rid of the g main loop??
	purple_eventloop_set_ui_ops(&p_data->glib_eventloops);

	// Set path to search for plugins. The core (libpurple) takes care of loading the
	// core-plugins, which includes the protocol-plugins. So it is not essential to add
	// any path here, but it might be desired, especially for ui-specific plugins.
	purple_plugins_add_search_path(CUSTOM_PLUGIN_PATH.c_str());

	// Now that all the essential stuff has been set, let's try to init the core. It's
	// necessary to provide a non-NULL name for the current ui to the core. This name
	// is used by stuff that depends on this ui, for example the ui-specific plugins.
	if (!purple_core_init(UI_ID.c_str()))
	{
		// Initializing the core failed. Terminate.
		p_debugLogCB("ERROR", "libpurple init failed, core dump");
		abort();
	}

	// Create and load the buddylist
	purple_set_blist(purple_blist_new());
	purple_blist_load();

	// Load the preferences
	purple_prefs_load();

	// Load the desired plugins. The client should save the list of loaded plugins in
	// the preferences using purple_plugins_save_loaded(PLUGIN_SAVE_PREF)
	purple_plugins_load_saved(PLUGIN_SAVE_PREF.c_str());

	// Load the pounces
	//	purple_pounces_load();

	p_debugLogCB("DEBUG", "Lib purple initialized, version " + ncstringutils::NCStringUtils::nullIsEmpty(purple_core_get_version()));

//	connect_to_signals();
	static int handle;


	// Setup signals
	p_debugLogCB("DEBUG", "Lib purple connecting signals");

	purple_signal_connect(purple_connections_get_handle(), "signed-on", &handle,
				PURPLE_CALLBACK(Data::signed_on), p_data.get());

	purple_signal_connect(purple_conversations_get_handle(), "received-im-msg", &handle,
			PURPLE_CALLBACK(Data::received_im_msg), p_data.get());

//	purple_signal_connect(purple_accounts_get_handle(), "account-authorization-requested", &handle,
//				PURPLE_CALLBACK(account_authorization_requested), NULL);

        purple_signal_connect(purple_blist_get_handle(), "buddy-signed-on", &handle,
				PURPLE_CALLBACK(Data::buddy_signed_on), p_data.get());

	// Setup account given user input
	p_data->purpleAccount.reset( purple_account_new(p_name.c_str(), p_protocol.c_str())  );
	purple_account_set_password(p_data->purpleAccount.get(), p_password.c_str());

	purple_accounts_add(p_data->purpleAccount.get());
	purple_account_set_enabled(p_data->purpleAccount.get(), UI_ID.c_str(), TRUE);

	p_debugLogCB("DEBUG", "Username: " + p_name);
	p_debugLogCB("DEBUG", "Protocol: " + p_protocol);

	p_data->start();

}


NCClientPurple::~NCClientPurple()
{
}

NCClientPurple::String NCClientPurple::getName()
{
	return p_name + "@" + p_protocol;
}

void NCClientPurple::connect()
{
}

void NCClientPurple::disconnect()
{
}

void NCClientPurple::sendTyping(const String &who, const String &msg, bool done)
{
}

void NCClientPurple::msgSend(const String &who, const String &msg)
{
	const int v = Data::send_msg(p_data->purpleAccount.get(), who.c_str(), msg.c_str());
	UNUSED(v);
}

void NCClientPurple::questionAnswerBool(const int id, const bool response)
{
}

void NCClientPurple::addBuddy(const String &who, const String &group)
{
}

void NCClientPurple::removeBuddy(const String &who, const String &group)
{
}

void NCClientPurple::setAway(const String &state, const String &msg)
{
}


void NCClientPurple::connectionProgress(const String &text, const int step, const int count)
{
	p_connectionStepCB(text, step, count);
}

void NCClientPurple::connected()
{
}

void NCClientPurple::disconnected()
{
}

void NCClientPurple::connectionNotice(const String &text)
{
}

void NCClientPurple::disconnectNotice(const String &text, const int reason)
{
}

void NCClientPurple::contactTyping()
{
}

void NCClientPurple::msgReceived(const String &who, const String &msg)
{
	p_msgReceivedCB(who, msg);
}

void NCClientPurple::questionAskedBool(const int id, const String &question)
{
}

void NCClientPurple::debugLog(const String &level, const String &logLine)
{
	p_debugLogCB(level, logLine);
}

} // namespace ncclientpurple
} // namespace ncpp

#endif
