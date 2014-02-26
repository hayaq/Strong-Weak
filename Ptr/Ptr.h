#ifndef __QC_PTR_H__
#define __QC_PTR_H__

#include <stddef.h>
#include <stdint.h>

namespace QC{
	
	template <class T>
	class Ptr{
		T *_ptr;
	public:
		Ptr() : _ptr(NULL){}
		Ptr(T *ptr) : _ptr(ptr){ incref(); }
		Ptr(const Ptr &ptr) : _ptr(ptr._ptr){ incref(); }
		Ptr& operator=(const Ptr& ptr){
			ptr.incref();
			decref();
			_ptr = ptr._ptr;
			return *this;
		}
		virtual ~Ptr(){ decref(); }
		T* ptr()const{ return _ptr; }
		operator bool()const{ return _ptr? true : false; }
		operator T*()const{ return (T*)_ptr; }
		operator void*()const{ return _ptr; }
		T* operator->()const{ return _ptr; }
		bool operator ==(const Ptr& ptr)const{ return _ptr==ptr._ptr; }
		bool operator !=(const Ptr& ptr)const{ return _ptr!=ptr._ptr; }
	private:
		void incref() const {
			if( _ptr ){ _ptr->_refc++; }
		}
		void decref(){
			if( _ptr && --_ptr->_refc==0 ){
				_ptr->Dealloc();
				_ptr = NULL;
			}
		}
	};
	
	class Obj;

	class WObj{
		int _refc;
		Obj *_wptr;
		friend class Obj;
		template<class> friend class Ptr;
		template<class> friend class WeakPtr;
	public:
		WObj():_refc(0), _wptr(NULL){}
		WObj(Obj *ptr):_refc(0), _wptr(ptr){}
		virtual ~WObj(){}
		Obj*& Ptr(){ return _wptr; }
		virtual void Dealloc(){
			if( this ){ delete this; }
		}
	};
	
	class Obj{
		int _refc;
		Ptr<WObj> _wptr;
		template<class> friend class Ptr;
		template<class> friend class WeakPtr;
	public:
		Obj():_refc(0),_wptr(){}
		virtual ~Obj(){
			if( _wptr ){ _wptr->_wptr = NULL; }
		}
		virtual void Dealloc(){
			if( this ){ delete this; }
		}
	};
	
	template <class T>
	class WeakPtr{
		Ptr<WObj> _wptr;
	public:
		WeakPtr() : _wptr(){}
		WeakPtr(T *ptr) : _wptr(WPtr(ptr)){}
		WeakPtr(const Ptr<T> &ptr) : _wptr(WPtr(ptr)){}
		WeakPtr<T>& operator=(const Ptr<T>& ptr){
			_wptr = WPtr(ptr);
			return *this;
		}
		WeakPtr<T>& operator=(T *ptr){
			_wptr = WPtr(ptr);
			return *this;
		}
		operator const Ptr<T>()const{ return Ptr<T>(ptr()); }
		T* ptr()const{ return _wptr? (T*)_wptr->_wptr : NULL; }
		operator bool()const{ return ptr()? true : false; }
		operator T*()const{ return (T*)ptr(); }
		operator void*()const{ return ptr(); }
		T* operator->()const{ return ptr(); }
		bool operator ==(const Ptr<T>& ptr)const{
			if( _wptr ){ return _wptr->_wptr==ptr; }
			return ptr.ptr()==NULL;
		}
		bool operator !=(const Ptr<T>& ptr)const{
			return !operator==(ptr);
		}
	private:
		Ptr<WObj> WPtr(T *ptr){
			Obj *o = (Obj*)ptr;
			if( !o ){ return NULL; }
			else if( !o->_wptr ){ o->_wptr = new WObj(o); }
			return o->_wptr;
		}
	};
	
};


#endif
