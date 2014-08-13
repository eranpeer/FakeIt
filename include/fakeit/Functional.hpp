namespace fakeit{

template< class C, typename R, typename... Args >
  class const_member_function;

template< class C, typename R, typename... Args >
  class const_member_function< C, R( Args... ) > {
    public:

      typedef R ( C::*type )( Args... );
      typedef const_member_function< C, R( Args... ) > this_type;

      const_member_function( ) {}
      const_member_function( type m ) : _m(m) {}

      this_type& operator=( type m ){ _m = m; return *this; }
      R operator() ( C& c, Args&&... args ){ c.*_m( std::forward<Args>(args)... ); }
      operator bool(){ return _m; }
      type operator*(){ return _m; }
    private:
      type _m = nullptr;
  };

template< class C, typename R, typename... Args >
  class member_function;

template< class C, typename R, typename... Args >
  class member_function< C, R( Args... ) > {
    public:

      typedef R ( C::*type )( Args... );
      typedef member_function< C, R( Args... ) > this_type;

      member_function( ) {}
      member_function( type m ) : _m(m) {}

      this_type& operator=( type m ){ _m = m; return *this; }
      R operator() ( C& c, Args&&... args ){ c.*_m( std::forward<Args>(args)... ); }
      operator bool(){ return _m; }
      type operator*(){ return _m; }
    private:
      type _m = nullptr;
  };

template< class C, typename R, typename... Args >
  class bound_member_function;

template< class C, typename R, typename... Args >
  class bound_member_function< C, R( Args... ) > {
    public:

      typedef R ( C::*type )( Args... );
      typedef bound_member_function< C, R( Args... ) > this_type;

      bound_member_function( ) {}
      bound_member_function( C& c, type m ) : _c(&c), _m(m) {}

      this_type& operator=( const std::pair<C&, type>& p ) {
        _c = &p.first;
        _m = &p.second;
        return *this;
      }

      R operator() ( Args&&... args ){ _c->*_m( std::forward<Args>(args)... ); }
      operator bool(){ return _m && _c; }
    private:
      C* _c;
      type _m = nullptr;
  };

template< class C, typename R, typename... Args >
  class bound_const_member_function;

template< class C, typename R, typename... Args >
  class bound_const_member_function< C, R( Args... ) > {
    public:

      typedef R ( C::*type )( Args... );
      typedef bound_const_member_function< C, R( Args... ) > this_type;

      bound_const_member_function( ) {}
      bound_const_member_function( C& c, type m ) : _c(&c), _m(m) {}

      this_type& operator=( const std::pair<C&, type>& p ) {
        _c = &p.first;
        _m = &p.second;
        return *this;
      }

      R operator() ( Args&&... args ){ _c->*_m( std::forward<Args>(args)... ); }
      operator bool(){ return _m && _c; }
    private:
      C* _c;
      type _m = nullptr;
  };
}

