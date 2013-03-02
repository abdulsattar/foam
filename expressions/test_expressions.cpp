
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "expressions.hpp"

template<typename TExpression>
void evaluate(char const *exprstr, TExpression e)
{
	std::cout <<"\n-------------------------------\n";
	std::cout << "Expression ( "<< exprstr << " )\n";
	for(size_t i = 1 ; i <= 10; ++i)
	{
		try 
		{
			std::cout << e(i) << std::endl;
		}
		catch(std::exception const & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

#define eval(e) evaluate(#e, e)

struct person 
{
	struct tag { std::string name; } proxy;
	int age;
	int get() const { return age; }
};

int main()
{
	try
	{
		snl::expr::expression<double> x;

		auto expr = x + 1.05;
		auto e = expr + 20.0;
		for(size_t i = 0 ; i < 10; ++i)
			std::cout << expr(i) <<", "<< e(i) << std::endl;
		eval( e + 100.5055 );
		eval( e + x + 100.5055 );
		eval( -100.15 + x + x );
		eval( x - x );
		eval( 5/x );
		eval( 5.0/x );
		eval( x * x - 4 * x  + 4 );
		eval( 1 / (x * x - 4 * x  + 4) );

		eval ( x > 4 );

		std::vector<person> ps 
		{ 
			{{"Sarfaraz"}, 10},
			{{"Nawaz"}, 20},
			{{"Mannu"}, 30},
			{{"Sannu"}, 40},
			{{"Nayab"}, 50}
		};
		{
			using snl::expr::expression;
			using snl::expr::member;
			using snl::expr::_m;

			auto age = _m(&person::age);
			auto cmp =  age > 30;
			for(auto p : ps )
				std::cout << p.age <<  " > 10 ="  <<  cmp(p)  <<", "<< (member(&person::age)(p)  - 100 )<< std::endl;
			std::cout << std::find_if(ps.begin(), ps.end(), age == 30 )->proxy.name << std::endl;

			std::vector<person> filtered;
			std::copy_if(ps.begin(), ps.end(), std::back_inserter(filtered), age >= 10 && age <= 40 );
			std::copy_if(ps.begin(), ps.end(), std::back_inserter(filtered), [](person const & p) { return p.age >= 10 && p.age <= 40; } );
			for(auto p : filtered)
				std::cout << "filtered = " <<  p.age  << std::endl; 

			std::vector<int> ages(ps.size());
			//std::transform(ps.begin(), ps.end(), ages.begin(), -2 * age * 8978 < 100);
			//std::transform(ps.begin(), ps.end(), ages.begin(),  age * 15);
			auto get = member(&person::get);
			std::transform(ps.begin(), ps.end(), ages.begin(),  get * 100);
			for(auto i : ages )  std::cout << "member function : " << i << std::endl;

			auto proxy = member(&person::proxy);
			auto name = member(&person::tag::name);
			auto size = member(&std::string::size);
			//auto chain = _m(&person::proxy) | _m(&person::tag::name) | _m(&std::string::size);
			auto chain = (proxy | name | size) * 100;
			int a  = (proxy | name | size) * 100;
			int *b = proxy | name | size * 100;
			std::cout << chain(ps[0]) << std::endl;
			std::cout << chain(ps[1]) << std::endl;
		}
	}
	catch(std::exception const & e)
	{
		std::cout << e.what() << std::endl;
	}
}