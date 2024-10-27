def Settings(**kwargs):
    return {
        'flags': [
            '-std=c++17',                            # Use C++17 standard
            '-Wall',                                 # Enable all warnings
            '-Wextra',                               # Enable extra warnings
            '-I./include',                           # Include path for your project headers
            '-I/home/ant/Desktop/OpenGLPractice/include',  # Full path to your GLAD headers
            '-I/usr/include',                        # System include path
            '-I/usr/local/include',                  # Local include path for libraries
            '-I/usr/include/GL',                     # Include path for OpenGL headers
        ],
        'do_cache': True,
    }
