#ifndef _LOG_HPP
#define _LOG_HPP

#include <iostream>

class Log {
    /** An extremely simple logging system. */
 public:
    /** Log level.  Typically if LEVEL > 0, you should output something. */
    static int LEVEL;
    /** Pointer to the stream where output should go.*/
    static std::ostream *stream;
    /** Redefine the output stream */
    static void set_output(std::ostream& stream);
    /** Access the output stream */
    static std::ostream& os();
};


#endif
