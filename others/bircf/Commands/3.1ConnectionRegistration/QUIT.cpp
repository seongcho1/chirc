#include "../../MessageManager.hpp"

void MessageManager::QUIT(int cs, std::vector<std::string>) {
  users_[cs].toQuit();
}

/*
https://datatracker.ietf.org/doc/html/rfc2813#section-4.1.5
4.1.5 Quit

      Command: QUIT
   Parameters: [<Quit Message>]

   A client session ends with a quit message.  The server MUST close the
   connection to a client which sends a QUIT message. If a "Quit
   Message" is given, this will be sent instead of the default message,
   the nickname or service name.

   When "netsplit" (See Section 4.1.6) occur, the "Quit Message" is
   composed of the names of two servers involved, separated by a space.
   The first name is that of the server which is still connected and the
   second name is either that of the server which has become
   disconnected or that of the server to which the leaving client was
   connected:

      <Quit Message> =  ":" servername SPACE servername

   Because the "Quit Message" has a special meaning for "netsplits",
   servers SHOULD NOT allow a client to use a <Quit Message> in the
   format described above.

   If, for some other reason, a client connection is closed without the
   client issuing a QUIT command (e.g. client dies and EOF occurs on
   socket), the server is REQUIRED to fill in the quit message with some
   sort of message reflecting the nature of the event which caused it to
   happen.  Typically, this is done by reporting a system specific
   error.

   Numeric Replies:

           None.

   Examples:

   :WiZ QUIT :Gone to have lunch   ; Preferred message format.


*/
