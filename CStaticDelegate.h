#pragma once

#include "IDelegate.h"

namespace Delegate
{
	//StaticDelegate ��ͨ������ί��

	template<typename ReturnType, typename ...ParamType>
	class CStaticDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:

		typedef  ReturnType(*Func)(ParamType...);

		CStaticDelegate(Func _func) : mFunc(_func) { }

		virtual bool isType(const std::type_info& _type) { return typeid(CStaticDelegate<ReturnType, ParamType...>) == _type; }

		virtual ReturnType invoke(ParamType ... params) { return mFunc(params...); }

		virtual bool compare(IDelegate<ReturnType, ParamType ...>* _delegate)const
		{
			if (0 == _delegate || !_delegate->isType(typeid(CStaticDelegate<ReturnType, ParamType ...>))) return false;
			CStaticDelegate<ReturnType, ParamType ...>* cast = static_cast<CStaticDelegate<ReturnType, ParamType ...>*>(_delegate);
			return cast->mFunc == mFunc;
		}

		virtual ~CStaticDelegate() {}
	private:
		Func mFunc;
	};


	//��ͨ������ί���ػ��汾
	template<typename ReturnType, typename ...ParamType>
	class CStaticDelegate<ReturnType(*)(ParamType ...)> :
		public IDelegate<ReturnType, ParamType ...>
	{
	public:

		//���� Func Ϊ void (void) ��������ָ�롣
		typedef  ReturnType(*Func)(ParamType...);

		CStaticDelegate(Func _func) : mFunc(_func) { }

		virtual bool isType(const std::type_info& _type) { return typeid(CStaticDelegate<ReturnType(*)(ParamType ...)>) == _type; }

		virtual ReturnType invoke(ParamType ... params) { return mFunc(params...); }

		virtual bool compare(IDelegate<ReturnType, ParamType ...>* _delegate)const
		{
			if (0 == _delegate || !_delegate->isType(typeid(CStaticDelegate<ReturnType(*)(ParamType ...)>))) return false;
			CStaticDelegate<ReturnType(*)(ParamType ...)>* cast = static_cast<CStaticDelegate<ReturnType(*)(ParamType ...)>*>(_delegate);
			return cast->mFunc == mFunc;
		}

		virtual ~CStaticDelegate() {}
	private:
		Func mFunc;
	};

	//��Ա����ί��
	template<typename T, typename ReturnType, typename ...ParamType>
	class CMethodDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		typedef ReturnType(T::* Method)(ParamType...);

		CMethodDelegate(T* _object, Method _method) : mObject(_object), mMethod(_method) { }

		virtual bool isType(const std::type_info& _type) { return typeid(CMethodDelegate<T, ReturnType, ParamType...>) == _type; }

		virtual ReturnType invoke(ParamType...params)
		{
			(mObject->*mMethod)(params...);
		}

		virtual bool compare(IDelegate<ReturnType, ParamType...>* _delegate) const
		{
			if (0 == _delegate || !_delegate->isType(typeid(CMethodDelegate<ReturnType, ParamType...>))) return false;
			CMethodDelegate<ReturnType, ParamType...>* cast = static_cast<CMethodDelegate<ReturnType, ParamType...>*>(_delegate);
			return cast->mObject == mObject && cast->mMethod == mMethod;
		}

		CMethodDelegate() {}
		virtual ~CMethodDelegate() {}
	private:
		T* mObject;
		Method mMethod;
	};

	//��Ա����ί���ػ�
	template<typename T, typename ReturnType, typename ...ParamType>
	class CMethodDelegate<T, ReturnType(T::*)(ParamType...)> :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		typedef ReturnType(T::* Method)(ParamType...);

		CMethodDelegate(T* _object, Method _method) : mObject(_object), mMethod(_method) { }

		virtual bool isType(const std::type_info& _type) { return typeid(CMethodDelegate<T, ReturnType(T::*)(ParamType...)>) == _type; }

		virtual ReturnType invoke(ParamType...params)
		{
			return (mObject->*mMethod)(params...);
		}

		virtual bool compare(IDelegate<ReturnType, ParamType...>* _delegate) const
		{
			if (0 == _delegate || !_delegate->isType(typeid(CMethodDelegate<T, ReturnType(T::*)(ParamType...)>))) return false;
			CMethodDelegate<T, ReturnType(T::*)(ParamType...)>* cast = static_cast<CMethodDelegate<T, ReturnType(T::*)(ParamType...)>*>(_delegate);
			return cast->mObject == mObject && cast->mMethod == mMethod;
		}

		CMethodDelegate() {}
		virtual ~CMethodDelegate() {}
	private:
		T* mObject;
		Method mMethod;
	};



    template< typename T>
    CStaticDelegate<T>* newDelegate(T func)
    {
        return new CStaticDelegate<T>(func);
    }
    template< typename T, typename F>
    CMethodDelegate<T, F>* newDelegate(T* _object, F func)
    {
        return new CMethodDelegate<T, F>(_object, func);
    }
}
