#ifndef __ARRAY_H__
#define __ARRAY_H__


#ifdef __cplusplus

#define BETWEEN(down, value, up)  \
                (unsigned(value) - down <= unsigned(up - down))

template<class Type>
class var_conf_array; 

template<class Type>
class var_conf_matrix; 

template<class Type>
class conf_array { 
  protected: 
    integer low_bound, high_bound; 
    Type    *buf;  
    
  public:
    conf_array(integer l, integer h) 
      : low_bound(l), high_bound(h) 
    { 
	buf = new Type[h-l+1];
    }
    conf_array(integer l, integer h, const Type* ptr) 
      : low_bound(l), high_bound(h) 
    { 
	buf = new Type[h-l+1];
	memcpy(buf, ptr, sizeof(Type)*(h-l+1));
    }
    conf_array(conf_array const& a) 
      : low_bound(a.low()), high_bound(a.high())
    {
	buf = new Type[size()];
	memcpy(buf, a.buf, sizeof(Type)*size());
    }
    conf_array(var_conf_array<Type> const& a);

    conf_array(const Type* ptr); // intialization with string

    ~conf_array() { delete[] buf; }	

    Type& operator[](integer index) {
        assert(BETWEEN(low_bound, index, high_bound));
	return buf[index-low_bound];
    }

    const Type& operator[](integer index) const {
        assert(BETWEEN(low_bound, index, high_bound));
	return buf[index-low_bound];
    }

    const integer low()  const { return low_bound; }
    const integer high() const { return high_bound; }
    const integer size() const { return high_bound - low_bound + 1; }

    operator Type*() { return buf; }

    operator Type const*() const { return buf; }
    
    void operator=(conf_array const& src) {
	assert(low_bound == src.low_bound && high_bound == src.high_bound); 
	memcpy(buf, src.buf, sizeof(Type)*size()); 
    }
    void operator=(var_conf_array<Type> const& src);

    friend char* lpsz(conf_array const& a) { 
	return lpsz(a.low_bound, a.high_bound, (char*)a.buf); 
    }
};

template<class Type>
class var_conf_array { 
  protected: 
    integer low_bound, high_bound; 
    Type    *buf;  
    
  public:
    var_conf_array(integer l, integer h) : low_bound(l), high_bound(h) 
    { 
	buf = new Type[h-l+1];
    }
    var_conf_array(integer l, integer h, Type* ptr) 
      : low_bound(l), high_bound(h), buf(ptr) {} 

    var_conf_array(conf_array<Type>& a) 
      : low_bound(a.low()), high_bound(a.high()), buf((Type*)a) {}

    var_conf_array(const Type* ptr); // intialization with string

    Type& operator[](integer index) {
        assert(BETWEEN(low_bound, index, high_bound));
	return buf[index-low_bound];
    }

    const Type& operator[](integer index) const {
        assert(BETWEEN(low_bound, index, high_bound));
	return buf[index-low_bound];
    }

    const integer low()  const { return low_bound; }
    const integer high() const { return high_bound; }
    const integer size() const { return high_bound - low_bound + 1; }

    operator Type*() { return buf; }
    operator Type const*() const { return buf; }
    
    void operator=(conf_array<Type> const& src) { 
	assert(low() == src.low() && high() == src.high()); 
	memcpy(buf, (Type const*)src, sizeof(Type)*size()); 
    }
    void operator=(var_conf_array const& src) { 
	assert(low() == src.low() && high() == src.high()); 
	memcpy(buf, (Type const*)src, sizeof(Type)*size()); 
    }
    friend char* lpsz(var_conf_array const& a) { 
	return lpsz(a.low_bound, a.high_bound, (char*)a.buf); 
    }
};

inline conf_array<char>::conf_array(const char* str) 
{
    int len = strlen(str);
    low_bound = 1;
    high_bound = len;
    buf = new char[len];
    memcpy(buf, str, len);  
}

template<class Type>
inline conf_array<Type>::conf_array(var_conf_array<Type> const& a) 
  : low_bound(a.low()), high_bound(a.high())
{
    buf = new Type[size()];
    memcpy(buf, (Type const*)a, sizeof(Type)*size());
}

template<class Type>
inline void conf_array<Type>::operator=(var_conf_array<Type> const& src) { 
    assert(low() == src.low() && high() == src.high()); 
    memcpy(buf, (Type const*)src, sizeof(Type)*size()); 
}

inline var_conf_array<char>::var_conf_array(const char* str) 
{
    low_bound = 1;
    high_bound = strlen(str);
    buf = (char*)str;
}


template<integer low_bound, integer high_bound, class Type>
class array {
  public:    
    Type buf[high_bound-low_bound+1];

    const integer low()  const { return low_bound; }
    const integer high() const { return high_bound; }

    const integer size() const { return high_bound - low_bound + 1; }

    Type& operator[](integer index) {
	assert(BETWEEN(low_bound, index, high_bound));     
	return buf[index-low_bound];
    }
    
    const Type& operator[](integer index) const {
	assert(BETWEEN(low_bound, index, high_bound));     
	return buf[index-low_bound];
    }
    
    operator Type*() { return buf; }
    operator Type const*() const { return buf; }

    Type& operator*() { return *buf; }

    operator conf_array<Type>() const { /* Convert to dynamic_array */
	return conf_array<Type>(low_bound, high_bound, buf);
    }      

    operator var_conf_array<Type>() {   /* Convert to dynamic_array */
	return var_conf_array<Type>(low_bound, high_bound, buf);
    }

    int compare( const Type* ptr) const { 
	return memcmp(buf, ptr, sizeof(buf)); 
    }
    
    boolean operator>(const array &arr) const { 
	return compare(arr.buf) > 0; 
    }
    boolean operator >= (const array &arr) const { 
	return compare(arr.buf) >= 0; 
    }
    boolean operator < (const array &arr) const { 
	return compare(arr.buf) < 0; 
    }
    boolean operator <= (const array &arr) const { 
	return compare(arr.buf) <= 0; 
    }
    boolean operator == (const array &arr) const { 
	return compare(arr.buf) == 0; 
    }
    boolean operator != (const array &arr) const { 
	return compare(arr.buf) != 0; 
    }
    
    boolean operator >  (const Type* ptr) const { 
	return compare(ptr) > 0; 
    }
    boolean operator >= (const Type* ptr) const { 
	return compare(ptr) >= 0; 
    }
    boolean operator < (const Type* ptr) const { 
	return compare(ptr) < 0; 
    }
    boolean operator <= (const Type* ptr) const { 
	return compare(ptr) <= 0; 
    }
    boolean operator == (const Type* ptr) const { 
	return compare(ptr) == 0; 
    }
    boolean operator != (const Type* ptr) const { 
	return compare(ptr) != 0; 
    }
    
    boolean operator >  (Type elem) const { 
	return compare(&elem) > 0; 
    }
    boolean operator >= (Type elem) const { 
	return compare(&elem) >= 0; 
    }
    boolean operator < (Type elem) const { 
	return compare(&elem) < 0; 
    }
    boolean operator <= (Type elem) const { 
	return compare(&elem) <= 0; 
    }
    boolean operator == (Type elem) const { 
	return compare(&elem) == 0; 
    }
    boolean operator != (Type elem) const { 
	return compare(&elem) != 0; 
    }
    
    friend text& operator << (text& t, array a)
    { 
	pio_output_string((text_descriptor*)&t, (char*)a.buf, 
			  high_bound-low_bound+1, NULL); 
	return t; 
    } 

    friend text& operator >> (text& t, array& a)
    {
	pio_input_string((text_descriptor*)&t, (char*)a.buf, 
		     high_bound-low_bound+1); 
	return t; 
    } 

    friend format_string<high_bound-low_bound+1> format(array const& a, 
							integer width)
    {
	return format_string<high_bound-low_bound+1>((char*)a.buf, width);
    }

#ifndef NO_ARRAY_ASSIGN_OPERATOR
    void operator = (const Type* ptr) { 
	memcpy(buf, ptr, sizeof(buf));
    }
    void operator = (Type elem) { 
	buf[0] = elem;
    }
#else   
    void assign(array const& a) { 
	memcpy(buf, a.buf, sizeof(buf));
    }
    void assign(const Type* ptr) { 
	memcpy(buf, ptr, sizeof(buf));
    }
    void assign(Type elem) { 
	buf[0] = elem;
    }
#endif    

    // constructor of array from string 
    static array make(Type const* ptr) { 
	array a;
	memcpy(a.buf, ptr, sizeof(a.buf));
	return a;
    }
    static array make(Type elem1, ...) { 
	va_list ap;
	int argno = 1;
	array a;
	va_start(ap, elem1);
	a.buf[0] = elem1;
	
	while(argno < high_bound-low_bound+1) { 
	    a.buf[argno++] = va_arg(ap, Type);
	}
	va_end(ap);
	return a;
    }
    friend char* lpsz(array const& a) { 
	return lpsz(low_bound, high_bound, (char*)a.buf); 
    }
};


// macro for construction array from string
#define as_array(s)  array<1,sizeof(s)-1,char>::make(s)

template<class Type>
class conf_matrix {
  protected:
    integer low_1, high_1, low_2, high_2;
    Type    *buf;

  public:     
    conf_matrix(integer l1, integer h1, integer l2, integer h2, Type const* p)
      : low_1(l1), high_1(h1), low_2(l2), high_2(h2)
    {
        buf = new Type[size()];
	memcpy(buf, p, sizeof(Type)*size());
    }
    conf_matrix(integer l1, integer h1, integer l2, integer h2)
      : low_1(l1), high_1(h1), low_2(l2), high_2(h2)
    {
        buf = new Type[size()];
    }
    conf_matrix(conf_matrix const& m) 
      : low_1(m.low1()), high_1(m.high1()), low_2(m.low2()), high_2(m.high2())
    {   
        buf = new Type[size()];
	memcpy(buf, (Type const*)m, sizeof(Type)*size());
    }	
    conf_matrix(var_conf_matrix<Type> const& m); 

    var_conf_array<Type> operator[](integer index) {
        assert(BETWEEN(low_1, index, high_1));
        return var_conf_array<Type>(low_2, high_2, 
				    buf+(index-low_1)*(high_2-low_2+1));
    }

    const var_conf_array<Type> operator[](integer index) const {
        assert(BETWEEN(low_1, index, high_1));
        return var_conf_array<Type>(low_2, high_2, 
			           (Type*)buf+(index-low_1)*(high_2-low_2+1));
    }

    void operator=(const conf_matrix& src) {
        assert(low_1 == src.low_1 && high_1 == src.high_1 &&
               low_2 == src.low_2 && high_2 == src.high_2);
	memcpy(buf, (const Type*)src, size()*sizeof(Type));
    }
    void operator=(const var_conf_matrix<Type>& src);

    const integer low1()  const { return low_1; }      
    const integer low2()  const { return low_2; }
    const integer high1() const { return high_1; }
    const integer high2() const { return high_2; }

    const integer size()  const { return (high_1 - low_1 + 1)
				        *(high_2 - low_2 + 1); }

    operator Type*() { return buf; }
    operator Type const*() const { return buf; }
};


template<class Type>
class var_conf_matrix {
  protected:
    integer low_1, high_1, low_2, high_2;
    Type    *buf;

  public:     
    var_conf_matrix(integer l1, integer h1, integer l2, integer h2, 
		    Type* ptr)
      : low_1(l1), high_1(h1), low_2(l2), high_2(h2), buf(ptr) {}

    var_conf_matrix(integer l1, integer h1, integer l2, integer h2)
      : low_1(l1), high_1(h1), low_2(l2), high_2(h2) 
    {
	buf = new Type[size()];
    }

    var_conf_matrix(conf_matrix<Type>& m) 
      : low_1(m.low1()), high_1(m.high1()), low_2(m.low2()), high_2(m.high2())
    {   
	buf = (Type*)m; 
    }	

    var_conf_array<Type> operator[](integer index) {
        assert(BETWEEN(low_1, index, high_1));
        return var_conf_array<Type>(low_2, high_2, 
				    buf+(index-low_1)*(high_2-low_2+1));
    }

    const var_conf_array<Type> operator[](integer index) const {
        assert(BETWEEN(low_1, index, high_1));
        return var_conf_array<Type>(low_2, high_2, 
				   (Type*)buf+(index-low_1)*(high_2-low_2+1));
    }

    void operator=(var_conf_matrix const& src) {
        assert(low1() == src.low1() && high1() == src.high1() &&
               low2() == src.low2() && high2() == src.high2());
	memcpy(buf, (Type const*)src, size()*sizeof(Type));
    }

    void operator=(conf_matrix<Type> const& src) { 
        assert(low1() == src.low1() && high1() == src.high1() &&
               low2() == src.low2() && high2() == src.high2());
	memcpy(buf, (Type const*)src, size()*sizeof(Type));
    }

    const integer low1()  const { return low_1; }      
    const integer low2()  const { return low_2; }
    const integer high1() const { return high_1; }
    const integer high2() const { return high_2; }

    const integer size()  const { return (high_1 - low_1 + 1)
				        *(high_2 - low_2 + 1); }

    operator Type*() { return buf; }
    operator Type const*() const { return buf; }
};


template<class Type>
conf_matrix<Type>::conf_matrix(var_conf_matrix<Type> const& m) 
  : low_1(m.low1()), high_1(m.high1()), low_2(m.low2()), high_2(m.high2())
{
    buf = new Type[size()];
    memcpy(buf, (Type const*)m, size()*sizeof(Type));
}	
 
template<class Type>
void conf_matrix<Type>::operator=(const var_conf_matrix<Type>& src) 
{
    assert(low1() == src.low1() && high1() == src.high1() &&
	   low2() == src.low2() && high2() == src.high2());
    memcpy(buf, (Type const*)src, size()*sizeof(Type));
}

    
/*
  Class matrix describes buf with fixed bounds.
*/

template<integer low_1, integer high_1, integer low_2, integer high_2, class Type>
class matrix { 
  public:
    array<low_2,high_2,Type> buf[high_1-low_1+1];

    array<low_2,high_2,Type>& operator[](integer index) {
	assert(BETWEEN(low_1, index, high_1));     
	return buf[index-low_1];
    }
    
    array<low_2,high_2,Type> const& operator[](integer index) const {
	assert(BETWEEN(low_1, index, high_1));     
	return buf[index-low_1];
    }
    
    operator conf_matrix<Type>() const {
        return conf_matrix<Type>(low_1, high_1, low_2, high_2, (Type*)buf);
    }
    operator var_conf_matrix<Type>() {
        return var_conf_matrix<Type>(low_1, high_1, low_2, high_2, 
				     (Type*)buf);
    }

    const integer low1()  const { return low_1; }
    const integer high1() const { return high_1; }
    const integer low2()  const { return low_2; }
    const integer high2() const { return high_2; }

    const integer size()  const { return (high_1 - low_1 + 1)
				        *(high_2 - low_2 + 1); }
    void assign(matrix const& m) { 
	memcpy(buf, m.buf, sizeof(buf));
    }
}; 

#else /* language C */

/*
 * macro for passing string as a function parameter together with bounds
 */
#define array(s)  1, sizeof(s)-1, (s)  

#define arrcmp(a,b) memcmp(a, b, sizeof(a))
#define arrcpy(a,b) memcpy(a, b, sizeof(a))

#endif


#endif 
