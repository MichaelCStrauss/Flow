def FlagsForFile( filename, **kwargs ):
    return {
        'flags': [ '-x', 'c++', '-I/Users/michael/Dev/flow/Fluid/include', '-I/Users/michael/Dev/flow/Flow/include', '-I/Users/michael/Dev/flow/glad/include', '-I/Users/michael/Dev/flow/glfw/include', '--std=c++1z', '-Wall', '-Wextra', '-Werror' ],
    }
