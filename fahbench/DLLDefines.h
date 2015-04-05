/* Making shared libraries on Windows is difficult.
 * 
 * CMake will define fahbench_EXPORTS
 *
 * Include this file and put FAHBENCH_EXPORT in class definitions
 * and static functions.
 */

#ifndef _fahbench_DLLDEFINES_H_
#define _fahbench_DLLDEFINES_H_

#if defined (_WIN32) 
#if defined(fahbench_EXPORTS)
#define  FAHBENCH_EXPORT __declspec(dllexport)
#else
#define  FAHBENCH_EXPORT __declspec(dllimport)
#endif /* fahbench_EXPORTS */

#else /* defined (_WIN32) */
#define FAHBENCH_EXPORT
#endif

#endif /* _fahbench_DLLDEFINES_H_ */