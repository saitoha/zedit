/* config.h - Zedit user configurable defines */

#if defined(DOS)
#define INT_IS_16BITS	1
#elif !defined(WIN32)
#define HAVE_TERMIOS	1
#define ZLIB		1	/* Requires zlib -lz */
#define SPELL		1	/* Requires libaspell */
#define UNDO		0	/* EXPERIMENTAL */
#endif

/* DOPIPES: Output in real time and you can continue editing.
 * DOPOPEN: Output in real time but you must wait until command complete.
 * default: No output or editing until command complete.
 */
#if defined(__unix__)
#define DOPIPES 1
#elif !defined(DOS)
#define DOPOPEN 1
#endif
