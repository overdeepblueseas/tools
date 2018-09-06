/*
 *	Print amortization schedule for fixed-rate mortgage
 *
 *	Usage:
 *
 *	    a.out principal APR [term [month year] ]
 *
 *	principal: amount borrowed ($)
 *	APR:	   annual percentage rate 
 *	term:	   term of mortgage (years); default = 30 (see TERM_YEARS)
 *	month:	   month (1-12) of first payment
 *	year:	   year (00-99) of first payment
 *
 *	If month and year are supplied, the month/year of each payment will be
 *	printed and YTD interest will be calculated on a calendar year basis.
 *
 *	Author: AW Rogers, Intermetrics
 *
 */

#define TERM_YEARS 30		/* default for loan term */
#define YES 1
#define NO 0

main(argc, argv)
int argc;
char *argv[];

{

/* Variable declarations: balance and all payments are long integers
   representing cents - converted to dollars only for printing */

float loan_amount, yearly_rate;	
double monthly_rate, x, y;
int term_years, term_months, month, year, i, month_and_year;
long balance, princ_payment, int_payment, total_interest, 
     ytd_interest, payment;

static char month_name[13][4] = {"   ", "Jan", "Feb", "Mar", "Apr",
            "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static char note = ' ';

/* Get the arguments from the command line; supply appropriate defaults */

if (argc < 3) {
    printf("usage: %s principal APR [term [month year]]\n", argv[0]);
    return;
    }
sscanf(argv[1], "%f", &loan_amount);
sscanf(argv[2], "%f", &yearly_rate);

if (argc >= 4)
    sscanf(argv[3], "%d", &term_years);
else
    term_years = TERM_YEARS;

if (argc >= 6) {
    sscanf(argv[4], "%d", &month);
    sscanf(argv[5], "%d", &year);
    if ((month < 1) || (month > 12)) month = 1;
    month_and_year = YES;
    }
else {
    month_and_year = NO;
    month = 1;
    year = 0;
    }

/* Initialize variables used in subsequent computations */

monthly_rate = yearly_rate / 1200.;
term_months = term_years * 12;
total_interest = 0;
ytd_interest = 0;
balance = loan_amount * 100.;

x = 1.0;	/* Determine monthly payment (in cents)	*/
for (i = 1; i <= term_months; ++i)
    x = x * (1.0 + monthly_rate);
y = (loan_amount * monthly_rate * x) / (x - 1);
payment = ((long) (y * 100. + 0.5));

/* Print header */

printf("\f\n\n");
if (! month_and_year)
    printf("    ");
printf(" Loan amount: $%.2f     APR: %.3f%%     Term: %d years",
	loan_amount, yearly_rate, term_years);
if (month_and_year)
    printf(" starting %s %02d", month_name[month], year % 100);

printf("\n\n");
if (month_and_year)
    printf("    ");
printf("%21d monthly payments of $%.2f \n",
    term_months, payment / 100.);

if (!month_and_year)
	exit(0);

printf("\n Pay't");
if (month_and_year)
    printf("   Date ");
printf("    Principal  Interest       Balance     Interest   Interest");

printf("\n  no. ");
if (month_and_year)
    printf("    due ");
printf("     Payment    Payment       of loan      (total)     (YTD)\n");

/* Perform amortization calculations for each month */

for (i = 1; i <= term_months; ++i) {
    int_payment = ((long) (balance * monthly_rate + 0.5));
    total_interest += int_payment;
    ytd_interest += int_payment;
    if (i == term_months) {
	payment = int_payment + balance;	/* final payment */
	note = '*';
	}
    princ_payment = payment - int_payment;
    balance -= princ_payment;

    printf("\n%5d%c  ", i, note);
    if (month_and_year)
	printf("%s %02d  ", month_name[month], year %100);
    printf("%10.2f%11.2f%14.2f%13.2f%11.2f", princ_payment / 100.,
	int_payment / 100., balance / 100., total_interest / 100.,
	ytd_interest / 100.);

    if (++month > 12) {		/* Keep track of month for YTD totals */
	month = 1;
	++year;
	ytd_interest = 0;
	if (i != term_months) printf("\n");
    }	
}		
printf("\n\n  %c Final payment: $%.2f\n", note, payment / 100.);
exit(0);
}
