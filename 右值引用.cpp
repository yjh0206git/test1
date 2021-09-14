#include<iostream>
#include<cstring>
#include<typeinfo>
#include<ctime>
#include<vector>
using namespace std;
class MyString{
	public:
		static size_t DCtor;
		static size_t Ctor;
		static size_t CCtor;
		static size_t CAsgn;
		static size_t MCtor;
		static size_t MAsgn;
		static size_t Dtor;
	private:
		char* _data;
		size_t _len;
		void _init_data(const char* s){
			_data=new char[_len+1];
			memcpy(_data,s,_len);
			_data[_len]='\0';
		}
	public:
		MyString():_data(NULL),_len(0){++DCtor;}
		
		MyString(const char* p):_len(strlen(p)){
			++Ctor;
			_init_data(p); 
		}
		
		MyString(const MyString& str):_len(str._len){
			++CCtor;
			_init_data(str._data); 
		}
		
		MyString(MyString&& str) noexcept
		:_len(str._len),_data(str._data){
			++MCtor;
			str._len=0;
			str._data=NULL;
		}
		
		MyString& operator=(const MyString& str){
			++CAsgn;
			if(this!=&str){
				if(_data) delete _data;
				_len=str._len;
				_init_data(str._data);
			}else{
				
			}
			return *this;
		}
		
		MyString& operator=(MyString&& str){
			++MAsgn;
			if(this!=&str){
				if(_data) delete _data;
				_len=str._len;
				_data=str._data;
				str._len=0;
				str._data=NULL;
			}
			return *this;
		}
		
		virtual _MyString(){
			++Dtor;
			if(_data){
				delete _data;
			}
		}
		
		bool operator<(const MyString& rhs) const{
			return string(this->_data)<string(rhs._data);
		} 
		
		bool operator==(const MyString& rhs) const{
			return string(this->_data)==string(rhs._data);
		} 
		
		char* get() const{return _data;}
};

size_t MyString::CCtor=0;
size_t MyString::Ctor=0;
size_t MyString::DCtor=0;
size_t MyString::CAsgn=0;
size_t MyString::MCtor=0;
size_t MyString::MAsgn=0;
size_t MyString::Dtor=0;

template<typename T>
void output_static_data(const T& myStr){
	cout<<typeid(myStr).name()<<"--"<<endl;
	cout<<"CCtor="<<T::CCtor<<" "
	    <<"Ctor="<<T::Ctor<<" "
	    <<"DCtor="<<T::DCtor<<" "
	    <<"MCtor="<<T::MCtor<<" "
	    <<"CAsgn="<<T::CAsgn<<" "
	    <<"MAsgn="<<T::MAsgn<<" "
	    <<"Dtor="<<T::Dtor
	    <<endl;
}

template<typename M>
void test_moveable(M c1,long& value){
	char buf[10];
	
	typedef typename iterator_traits<typename M::iterator>::value_type V1type;
	clock_t timeStart=clock();
	for(long i=0;i<value;i++){
		snprintf(buf,10,"%d",rand());
		auto ite=c1.end();
		c1.insert(ite,V1type(buf));
	}
	cout<<"construction,milli-seconds:"<<(clock()-timeStart)<<endl;
	cout<<"size()="<<c1.size()<<endl;
	output_static_data(*(c1.begin()));
	
	timeStart=clock();
	M c11(c1);
	cout<<"copy,milli-seconds:"<<(clock()-timeStart)<<endl;
	
	timeStart=clock();
	M c12(move(c1));
	cout<<"move,milli-seconds:"<<(clock()-timeStart)<<endl;
	
	timeStart=clock();
	c11.swap(c12);
	cout<<"swap,milli-seconds:"<<(clock()-timeStart)<<endl;
}

int main(){
	long value=3000000;
	test_moveable(vector<MyString>(),value);
	return 0; 
} 
