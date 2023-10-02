#include "cbase.h"
#include "vstdlib/random.h"


CON_COMMAND(sb_equal, "Returns 1 if the two values are equal, otherwise returns 0\nUSAGE: sb_equal <VALUE A> <VALUE B>")
{
	if (args.ArgC() <= 2)
	{
		Msg("USAGE: sb_equal <VALUE A> <VALUE B>\n");
		return;
	}
	if (strcmp(args[1], args[2]) == 0)
	{
		Msg("1\n");
	}
	else
	{
		Msg("0\n");
	}
}

CON_COMMAND(sb_greater, "Returns 1 if the the first value is greater than the second value, otherwise returns 0\nUSAGE: sb_greater <VALUE A> <VALUE B>")
{
	if (args.ArgC() <= 2)
	{
		Msg("USAGE: sb_greater <VALUE A> <VALUE B>\n");
		return;
	}
	if(atoi(args[1]) > atoi(args[2]))
	{
		Msg("1\n");
	}
	else
	{
		Msg("0\n");
	}
}

CON_COMMAND(sb_lesser, "Returns 1 if the the first value is lesser than the second value, otherwise returns 0\nUSAGE: sb_lesser <VALUE A> <VALUE B>")
{
	if (args.ArgC() <= 2)
	{
		Msg("USAGE: sb_lesser <VALUE A> <VALUE B>\n");
		return;
	}
	if (atoi(args[1]) < atoi(args[2]))
	{
		Msg("1\n");
	}
	else
	{
		Msg("0\n");
	}
}

CON_COMMAND(sb_random_int, "Returns a random integer in a range\nUSAGE: sb_random_int <MIN> <MAX>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_random_int <MIN> <MAX>\n");
		return;
	}
	Msg("%i\n", random->RandomInt(atoi(args[1]), atoi(args[2])));
}

CON_COMMAND(sb_random_float, "Returns a random float in a range\nUSAGE: sb_random_float <MIN> <MAX>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_random_float <MIN> <MAX>\n");
		return;
	}
	Msg("%f\n", random->RandomFloat(atof(args[1]), atof(args[2])));
}

CON_COMMAND(sb_add, "Add two values\nUSAGE: sb_add <A> <B>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_add <A> <B>\n");
		return;
	}
	Msg("%f\n", atof(args[1])+atof(args[2]));
}

CON_COMMAND(sb_subtract, "Subtract two values\nUSAGE: sb_subtract <A> <B>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_subtract <A> <B>\n");
		return;
	}
	Msg("%f\n", atof(args[1]) - atof(args[2]));
}

CON_COMMAND(sb_multiply, "Multiply two values\nUSAGE: sb_multiply <A> <B>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_multiply <A> <B>\n");
		return;
	}
	Msg("%f\n", atof(args[1]) * atof(args[2]));
}

CON_COMMAND(sb_divide, "Divide two values\nUSAGE: sb_divide <A> <B>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_divide <A> <B>\n");
		return;
	}
	Msg("%f\n", atof(args[1]) / atof(args[2]));
}

CON_COMMAND(sb_pow, "Raise A to the power of B\nUSAGE: sb_pow <A> <B>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_pow <A> <B>\n");
		return;
	}
	Msg("%f\n", powf(atof(args[1]), atof(args[2])));
}

CON_COMMAND(sb_nthroot, "Take the Bth root of A\nUSAGE: sb_root <A> <B>")
{
	if (args.ArgC() <= 2) {
		Msg("USAGE: sb_nthroot <A> <B>\n");
		return;
	}
	Msg("%f\n", powf(atof(args[1]),1/atof(args[2])));
}

CON_COMMAND(sb_floor, "Round a value down\nUSAGE: sb_floor <A>")
{
	if (args.ArgC() <= 1) {
		Msg("USAGE: sb_floor <A>\n");
		return;
	}
	Msg("%i\n", Floor2Int(atof(args[1])));
}

CON_COMMAND(sb_round, "Round a value to the nearest integer\nUSAGE: sb_round <A>")
{
	if (args.ArgC() <= 1) {
		Msg("USAGE: sb_round <A>\n");
		return;
	}
	Msg("%i\n", RoundFloatToInt(atof(args[1])));
}

CON_COMMAND(sb_ceil, "Round a value up\nUSAGE: sb_ceil <A>")
{
	if (args.ArgC() <= 1) {
		Msg("USAGE: sb_ceil <A>\n");
		return;
	}
	Msg("%i\n", Ceil2Int(atof(args[1])));
}

CON_COMMAND(sb_cos, "Take a cosine of a value\nUSAGE: sb_cos <A>")
{
	if (args.ArgC() <= 1) {
		Msg("USAGE: sb_cos <A>\n");
		return;
	}
	Msg("%f\n", cosf(atof(args[1])));
}

CON_COMMAND(sb_sin, "Take a cosine of a value\nUSAGE: sb_sin <A>")
{
	if (args.ArgC() <= 1) {
		Msg("USAGE: sb_sin <A>\n");
		return;
	}
	Msg("%f\n", sinf(atof(args[1])));
}

CON_COMMAND(sb_tan, "Take a cosine of a value\nUSAGE: sb_tan <A>")
{
	if (args.ArgC() <= 1) {
		Msg("USAGE: sb_tan <A>\n");
		return;
	}
	Msg("%f\n", tanf(atof(args[1])));
}

CON_COMMAND(sb_lerp, "Linearly interpolate between two values\nUSAGE: sb_lerp <A> <B> <I>")
{
	if (args.ArgC() <= 3) {
		Msg("USAGE: sb_lerp <A> <B> <I>\n");
		return;
	}
	float i = atof(args[3]);
	Msg("%f\n", (1-i)*atof(args[1]) + i*atof(args[2]) );
}
