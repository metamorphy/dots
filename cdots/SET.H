#ifndef __SET_H__
#define __SET_H__

#define eos (MAX_SET_CARD)                /* end of set marker */
#define INTERVAL_MARKER  (MAX_SET_CARD+1) /* interval marker */

#define range(a,b) (a), INTERVAL_MARKER, (b)

typedef unsigned char SetElemType;
typedef integer       SetArrayElemType;
 
#define BITS_PER_WORD (sizeof(SetArrayElemType)*8)
#define ALL_BITS_SET  -1L
#define MAX_SET_CARD 256

#ifdef __cplusplus


template<int n>
class set_template {
  public:
    SetArrayElemType setarray[n];

    static set_template of(int param1,...) {  
	set_template s = {{0}};
	SetElemType elem = param1;
	boolean interval = false;
	int param = param1;
	va_list ap;
	unsigned i;
	
	va_start(ap, param1);
	
	while (param != eos) {
	    
	    if (interval) {
		unsigned low  = (SetElemType)elem;
		unsigned high = (SetElemType)param;
		unsigned lowindex  = low / BITS_PER_WORD;
		unsigned highindex = high / BITS_PER_WORD;
		
		if(lowindex == highindex) {
		    s.setarray[lowindex] |= ((1L << (high-low+1)) - 1) 
						<< (low % BITS_PER_WORD); 
		} else {
		    s.setarray[lowindex] |= ALL_BITS_SET 
			                        << (low % BITS_PER_WORD);
		    
		    for(i = lowindex+1; i < highindex; i++)
			s.setarray[i] = ALL_BITS_SET;
		    
		    s.setarray[highindex] = (1L << (high % BITS_PER_WORD)) - 1;
		}    
		interval = false;
		elem = param; 
	    }  else if(param == INTERVAL_MARKER) { 
		interval = true;
	    } else { /* single value */
		elem = param; 
		s.setarray[(unsigned)elem / BITS_PER_WORD] 
		    |= 1L << ((unsigned)elem % BITS_PER_WORD); 
	    }
	    param = va_arg(ap, int);         
	}
	va_end(ap);
	return s;
    }
};

typedef set_template<MAX_SET_CARD/BITS_PER_WORD> set;

#define set_of_enum(e) set_template<(last_##e-1 + BITS_PER_WORD-1) \
                                    / BITS_PER_WORD>


#else /* language C */

#define SET_LENGTH (MAX_SET_CARD/BITS_PER_WORD)

typedef struct { 
    long setarray[SET_LENGTH];
} set;

boolean subset(set a, set b);    /* if <a> is subset of <b> */
boolean inset(SetElemType elem, set s);
boolean equivalent(set a, set b); 
set     setof(int elem1, ...);
set     join(set a, set b);
set     difference(set a, set b);
set     intersect(set a, set b);
 
typedef unsigned int   set32;
typedef unsigned short set16;

#define ELEM(e)         (1L << (e))
#define INSET(e,s)      ((ELEM(e) & (s)) != 0)
#define DIFFERENCE(a,b) ((a) & ~(b))
#define	JOIN(a,b)       ((a) | (b))
#define INTERSECT(a,b)  ((a) & (b))
#define SUBSET(a,b)     (((a) & ~(b)) == 0)
#define EQUIVALENT(a,b) ((a) == (b))

#define RANGE(a,b)      (ELEM((a)+1) - ELEM(b))


#endif

#endif /* module */
