#ifndef ERROR_H
# define ERROR_H

# ifdef DEBUG_MODE
#  define DEBUG(msg) (std::cout << "[DEBUG " << __FILE__ << "@" << __LINE__ << "] " << msg << std::endl)
# else
#  define DEBUG(msg) (NULL)
# endif

#endif
