#ifndef GL_ERROR_HPP
#define GL_ERROR_HPP

#include <string>

using namespace std;

/** Debugging functions.
 *
 * After any openGL call, you can call one of the check() functions to report
 * openGL errors that may have occured.
 */
class GL_Error {
public:
    /** Ask OpenGL if any errors occured from the last API call.
     *
     * If there's an error, prints an error message,
     * and calls die_or_continue().
     * If not, returns with no action.
     *
     * Calls this OpenGL function:
```
glError : returns most recent error code.
```
     * @param msg Extra info to be printed.
     */
    static void check(const std::string& msg);

    /** Ask OpenGL if any errors occured in compiling a vertex
     * or fragment shader.
     *
     * If there were compilation errors, prints them, and calls
     * die_or_continue().   If not, returns with no action.
     *
     * Calls these OpenGL functions:
```
glGetShaderiv      : reports if a compilation error occured
glGetShaderInfoLog : gets the text of the compilation error(s).
```
     *
     * @param msg Extra info to be printed.
     * @param shader_handle Needed to query openGL about the errors.
     */

    static void check_compiled_OK(const string& msg, int shader_handle,
                                  const string& file_name);

    /** Ask OpenGL if any errors occured when linking together
     * a vertex shader and a fragment shader.
     *
     * If there were link errors, prints them, and calls die_or_continue().
     * If not, returns with no action.
     *
     * Calls these OpenGL functions:
```
glGetProgramv      : reports if a linking error occured
glGetShaderInfoLog : gets the text of the linking error(s).
```
     *
     * @param msg Extra info to be printed.
     * @param program_handle Needed to query openGL about the errors.
     */
    static void check_linked_OK(const string& msg, int program_handle,
                                const string& vertex_shader_file,
                                const string& fragment_shader_file);

    /** Prints an error message, and then returns
     *  or waits for user to kill program.
     *
     * Behavior depends on die_on_error flag.  If false, returns.
     * If true, waits for user to kill program.
     *
     * @param message Error message to be printed.
     */
    static void die_or_continue(const string& message="OpenGL Error");

    /** Determines whether die_on_error should keep going,
     * or wait for user to kill program.
     */
    static bool die_on_error;


    // /** Callback for OpenGL errors.
    //  * I can't get this to work on a Mac (GL_DEBUG_OUTPUT is undefined).
    //  */
    // static void gl_error_callback(GLenum source,
    //                               GLenum type,
    //                               GLuint id,
    //                               GLenum severity,
    //                               GLsizei length,
    //                               const GLchar* message,
    //                               const void* userParam);
};

#endif
