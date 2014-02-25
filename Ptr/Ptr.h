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
		~Ptr(){ decref(); }
		operator bool()const{ return _ptr? true : false; }
		operator T*()const{ return (T*)_ptr; }
		T* operator->(){ return _ptr; }
		bool operator ==(const Ptr& ptr)const{ return _ptr==ptr._ptr; }
		bool operator !=(const Ptr& ptr)const{ return _ptr!=ptr._ptr; }
	private:
		void incref() const {
			if( _ptr ){ _ptr->Retain(); }
		}
		void decref(){
			if( _ptr ){ _ptr = _ptr->template Release<T>(); }
		}
	};
	
	template<class> class WeakPtr;
	
	class Obj{
		int _refc;
		int _weak;
		template<class> friend class WeakPtr;
	public:
		Obj():_refc(0),_weak(0){}
		virtual ~Obj(){}
		int  RefC(){ return _refc; }
		Obj* Retain(){ return (this && ++_refc)? this : NULL; }
		template<class T> T* Release(){
			if( this && --_refc==0 ){
				if( _weak ){
					WeakPtr<T>::Unreg((T*)this);
				}
				delete this;
				return NULL;
			}
			return (T*)this;
		}
	};
	
	template <class T>
	class WeakPtr{
		friend QC::Obj;
		class WObj : public QC::Obj{
		public:
			T *_ptr;
			WObj *_next;
			WObj(T* ptr=NULL):_ptr(ptr),_next(NULL){
				((Obj*)_ptr)->_weak = 1;
			}
			~WObj(){ Unreg(_ptr); }
		};
		Ptr<WObj> _ptr;
	public:
		WeakPtr() : _ptr(){}
		WeakPtr(T *ptr) : _ptr(Reg(ptr)){}
		WeakPtr(const Ptr<T> &ptr) : _ptr(Reg(ptr)){}
		WeakPtr<T>& operator=(const Ptr<T>& ptr){
			_ptr = Reg(ptr);
			return *this;
		}
		WeakPtr<T>& operator=(T *ptr){
			_ptr = Reg(ptr);
			return *this;
		}
		operator bool()const{ return (_ptr&&_ptr._ptr)? true : false; }
		operator T*(){ return (T*)(_ptr?_ptr->_ptr : NULL); }
		T* operator->(){ return (T*)(_ptr?_ptr->_ptr : NULL); }
		bool operator ==(const Ptr<T>& ptr)const{ return _ptr==ptr._ptr; }
		bool operator !=(const Ptr<T>& ptr)const{ return _ptr!=ptr._ptr; }
	private:
		static const int N = 251;
		static WObj** Table(){
			static int init = 0;
			static WObj* tbl[N];
			if( !init ){
				for(int i=0;i<N;i++){ tbl[i]=NULL; }
				init = 1;
			}
			return tbl;
		}
		static Ptr<WObj> Reg(T* ptr){
			if( !ptr ){ return NULL; }
			WObj *obj = Get(ptr);
			if( !obj ){
				obj = new WObj(ptr);
				WObj **item0 = Table()+(uintptr_t)ptr%N;
				if( *item0 != NULL ){
					WObj *item = *item0;
					while(item->_next!=NULL){
						item = item->_next;
					}
					item->_next = obj;
				}else{
					*item0 = obj;
				}
			}
			return Ptr<WObj>(obj);
		}
		static void Unreg(T* ptr){
			if( !ptr ){ return; }
			WObj **item0 = Table()+(uintptr_t)ptr%N;
			WObj *item = *item0;
			WObj *prev = NULL;
			while (item) {
				if( item->_ptr == ptr ){
					if( prev ){
						prev->_next = item->_next;
					}else{
						*item0 = item->_next;
					}
					item->_ptr = NULL;
					return;
				}
				prev = item;
				item = item->_next;
			}
		}
		static WObj* Get(T *ptr){
			WObj *item = Table()[(uintptr_t)ptr%N];
			while (item) {
				if( item->_ptr==ptr ){ return item; }
				item = item->_next;
			}
			return NULL;
		}
	};
	
};


#endif
