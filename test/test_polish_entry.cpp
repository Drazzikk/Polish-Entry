#include "gtest.h"
#include "polish_entry.h"

TEST(Number, can_create_number)
{
	ASSERT_NO_THROW(Number a(1));
}

TEST(Number, can_get_number_value)
{
	Number a(1);
	EXPECT_EQ(a.getValue(), 1);
}

TEST(Operation, can_create_operation)
{
	ASSERT_NO_THROW(Operation a('*'));
}

TEST(Operation, can_get_operation)
{
	Operation a('*');
	EXPECT_EQ(a.getOperation(), '*');
}

TEST(Close_Bracket, can_create_close_bracket)
{
	ASSERT_NO_THROW(Close_Bracket a);
}

TEST(Open_Bracket, can_create_open_bracket)
{
	ASSERT_NO_THROW(Open_Bracket a);
}

TEST(Translator, can_create_translator)
{
	ASSERT_NO_THROW(Translator a("1+1"));
}

TEST(Translator, can_fixed_analysis)
{
	Translator a("1+1");
	ASSERT_NO_THROW(a.fixed_analysis());
}

TEST(Translator, throw_when_not_correct_fixed_analysis)
{
	Translator a("1+m1");
	ASSERT_ANY_THROW(a.fixed_analysis());
}

TEST(Translator, throw_when_first_is_open_bracket)
{
	Translator a(")1+1");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, can_syntax_analysis)
{
	Translator a("1+1");
	a.fixed_analysis();
	ASSERT_NO_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_first_is_operation)
{
	Translator a("+1+1");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_last_is_operation)
{
	Translator a("1+1+");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, not_throw_when_correct_brackets)
{
	Translator a("(1+1)+(1+(1+1))");
	a.fixed_analysis();
	ASSERT_NO_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_not_correct_brackets)
{
	Translator a("(1+1)+(1+(1+1)))");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_operation_after_operation)
{
	Translator a("1++1");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_operand_after_operand)
{
	Translator a("1 1");
	ASSERT_ANY_THROW(a.fixed_analysis());
}

TEST(Translator, throw_when_open_bracket_after_operand)
{
	Translator a("1(");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_close_bracket_after_operation)
{
	Translator a("1+)");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_operation_after_open_bracket)
{
	Translator a("(+1)");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_operand_after_close_bracket)
{
	Translator a("(1+1)2");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_open_bracket_after_close_bracket)
{
	Translator a("(1+1)(2+2)");
	a.fixed_analysis();
	ASSERT_ANY_THROW(a.syntax_analysis());
}

TEST(Translator, throw_when_div_on_zero)
{
	Translator a("1+1/0");
	ASSERT_ANY_THROW(a.solution());
}

TEST(Translator, right_answer_with_int)
{
	Translator a("1+4-5*(3+(6/3))");
	EXPECT_EQ(a.solution(), -20);
}

TEST(Translator, right_answer_with_double)
{
	Translator a("1.1+4.3-5.0*(3.5+(9.9/3.3))");
	EXPECT_EQ(a.solution(), -27.1);
}

TEST(Translator, rigth_answer_with_big_expression)
{
	Translator a("(15.0+(3.0*(2.0+1.0)))*(4.0-(2.0/(1.0+1.0)))+(5.5*(3.0+2.0))*(2.6+6.4)");
	EXPECT_EQ(a.solution(), 319.5);
}
