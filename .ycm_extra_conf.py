import os
import os.path
import fnmatch

flags = [
    '-Wall',
    '-Wextra',
    '-Werror',
    '-I', '/home/Anthony/glfw-project/include/glad',  # This is where your glad.h file is located
    '-I', '/home/Anthony/glfw-project/include/KHR'
    '-std=c++17',       # Assuming you are working with C++
    '-x', 'c++',
    '-I/usr/include',   # System includes
    '-I/usr/local/include', # Other system includes if needed
]

def FlagsForFile( filename, **kwargs ):
    return {
        'flags': flags,
        'do_cache': True
    }
