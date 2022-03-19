#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>


COORD coord= {0,0}; // this is global variable
void gotoxy(int x,int y) //to set the cursor at given row and column value
{
	coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void clrscr() //to clear the output screen
{
    system("cls");
}


struct Customer  //class for customers
{
  	int x=0;        //x,y are position coordinates; d_x,d_y are destination coordinates
  	int y=0;
  	int d_x=0;
  	int d_y=0;                    
};

void Customer(struct Customer &c, int x, int y, int d_x, int d_y) //Constructor for Customer
{
	c.x = x;
	c.d_x = d_x;
	c.y = y;
	c.d_y = d_y;
}

void prior(struct Customer &c) //Updates the Customer's coordinates to their destination coordinates after they finish their ride
{
	c.x = c.d_x;
	c.y = c.d_y;
}

int get_distance(int x1, int y1, int x2, int y2) //gives |x2-x1|+|y2-y1|
{
  if (x1 >= x2 && y1 >= y2)
    return x1 - x2 + y1 - y2;
  else if (x1 >= x2 && y2 >= y1)
    return x1 - x2 + y2 - y1;
  else if (x2 >= x1 && y1 >= y2)
    return x2 - x1 + y1 - y2;
  else
    return x2 - x1 + y2 - y1;
}

struct Cab //class for cab
{
	int base_cost, cost_per_km, base_distance;  // base_cost -- is the cost before base_distance
	int x, y, license, total_rides;    // x,y are coordinates of the car, total_rides is total number of rides of the car so far
  	double rating;     // rating is the rating (out of 5) of the car so far                                
    char name[10], type[10];   //name is name of car and type is the category
};

void Cab(struct Cab &c, char n[], int x, int y, int l, float r, int t) //Constructor that assigns all values, including type of the car based on name
{ 
	c.x = x;
	c.y = y;
	c.license = l;
	c.rating = r;
	c.total_rides = t;
	strcpy(c.name,n);

	if (strcmp(n,"Dzire")==0 || strcmp(n,"Amaze")==0 || strcmp(n,"Vento")==0 || strcmp(n,"City")==0 || strcmp(n,"Verna")==0 || n == "Rapid" || strcmp(n,"Ciaz")==0)
	{
	  strcpy(c.type,"Sedan");
	  c.base_distance = 5;
	  c.base_cost = 80;
	  c.cost_per_km = 15;
	}
	else if (strcmp(n,"i20")==0 || strcmp(n,"i10")==0 || strcmp(n,"Swift")==0 || strcmp(n,"Baleno")==0 || strcmp(n,"Kwid")==0 || strcmp(n,"WagonP")==0 || strcmp(n,"Polo")==0)
	{
	  strcpy(c.type , "Hatchback");
	  c.base_distance = 10;
	  c.base_cost = 100;
	  c.cost_per_km = 13;
	}
	else if (strcmp(n,"Innova")==0 || strcmp(n,"Brezza")==0 || strcmp(n,"Seltos")==0 || strcmp(n,"Sonet")==0 || strcmp(n,"Creta")==0 || strcmp(n,"Ertiga")==0 || strcmp(n,"Fortuner")==0)
	{
	  strcpy(c.type , "SUV");
	  c.base_distance = 8;
	  c.base_cost = 125;
	  c.cost_per_km = 18;
	}
	else if (strcmp(n,"BMW")==0 || strcmp(n,"Audi")==0 || strcmp(n,"Mercedes")==0 || strcmp(n,"LandRover")==0 || strcmp(n,"Jaguar")==0 || strcmp(n,"Porsche")==0 || strcmp(n,"Lexus")==0)
	{
	  strcpy(c.type , "Luxury");
	  c.base_distance = 15;
	  c.base_cost = 175;
	  c.cost_per_km = 20;
	}
}

int get_distance_from_person(struct Customer &c, struct Cab &b)  // gives the distance of the cab from the customer
{
	return get_distance(c.x, c.y, b.x, b.y);
}

int get_cost(struct Customer &c, struct Cab &b) //gives cost of the taxi ride of this cab for customer c
{
	int d = get_distance_from_person(c,b) +   //Total ditance travelled by car = distance from person + distance from person to destination
	        get_distance(c.x, c.y, c.d_x, c.d_y);
	if (d <= b.base_distance)
	  return b.base_cost;
	else
	  return (((d - b.base_distance) * b.cost_per_km) + b.base_cost);
}

void print_values(struct Customer &c, struct Cab &b)  //to print all the details of the cab
{
	printf("Cab: %s \tLicense: %d \tLocated at (%d,%d) \tDistance from you: %d",b.name,b.license,b.x,b.y,get_distance_from_person(c,b));
}

void rate(struct Cab &c) //Allows user to rate their ride and updates the rating of the car as well as total rides
{
	printf("Rate your ride from 1-5 : ");
	double r;
	scanf("%lf",&r);
	// float total_sum = (float) c.rating * c.total_rides + r;
	c.total_rides++;
	c.rating = c.rating + (r/c.total_rides);
}

double driver_rate(struct Cab &c) //This is for when user has logged in as a taxi driver, provides a random rating to the driver and updates the rating
{
	srand(time(0));
	double r = 1 + (rand() % (5));
	c.total_rides++;
	c.rating = c.rating + (r/c.total_rides);
	return r;
}

int journey(struct Cab arr[], struct Customer &d) //provides the closest car from v to Person p and returns the cost
{
	struct Cab c = arr[0];
	int min = get_distance_from_person(d,c);
	int f = 0;
	clrscr();
	gotoxy(5,2);
	printf("Cars available in your chosen category are: ");

	for (int i=0;i<10;i++)
	{
		c = arr[i];
		if (get_distance_from_person(d,c) < min)  //if arr[i] cab is closer, update least distance
		{
		  min = get_distance_from_person(d,c);
		  f = i;
		}
		gotoxy(5,i+4);
		print_values(d,c);  //prints all available cars
	}

	gotoxy(5,16);
	printf("===================================================================================");
	gotoxy(10,17);
	printf("The closest car to you has been chosen, which is: ");
	gotoxy(10,18);
	print_values(d,arr[f]);
	gotoxy(10,20);
	printf("Total cost of journey is Rs.%d" ,get_cost(d,arr[f]));
	gotoxy(10,21);
	printf("Driver will reach you in %d minutes. Enjoy your ride!\n", get_distance_from_person(d,arr[f])*2);
	gotoxy(5,22);
	printf("===================================================================================");

	int cost1 = get_cost(d,arr[f]);		//total cost of ride
	prior(d);	//update coordinates of customer

	Sleep(8000);	//pause the output screen for 8 seconds
	clrscr();
	gotoxy(5,2);
	printf("Your ride is finished! You are now at %d,%d ", d.x, d.y);
	gotoxy(5,3);
	rate(arr[f]);	//rating the cab ride

	return cost1;
}

bool isValid(int n, char ref[])		//checks the validity of reference code that should have a lower case and upper case alphabet, a digit and a special character
{
    int d=0,u=0,l=0,s=0;

    for(int i=0;i<n;i++){
        if(ref[i]<=122 && ref[i]>=97)
            l=1;
        else if(ref[i]>=65 && ref[i]<=90)
            u=1;
        else if(ref[i]>=48 && ref[i]<=57)
            d=1;
        else
            s=1;
    }

    if((l+u+d+s)==4)
        return true;
    else
        return false;
}

void scan_data(struct Cab Arr[], FILE &f) //reads data from input file and stores it in array arr
{ 
	for (int i = 0; i < 10; i++)
	{
		struct Cab c;
		char name[10];
		int x, y, l, t;
		double r=0.0;
		fscanf(&f,"%s %d %d %d %lf %d",&name,&x,&y,&l,&r,&t);
		Cab(c,name, x, y, l, r, t);
		Arr[i] = c;
	}

}

void print_data(struct Cab Arr[], FILE &f) //reads data from array arr and writes it into the file
{
	for (int i = 0; i < 10; i++)
	{
		struct Cab c = Arr[i];
		fprintf(&f,"%s %d %d %d %lf %d\n",c.name,c.x,c.y,c.license,c.rating,c.total_rides);
	}
}




int main()
{
	//declaring arrays of struct cab
	struct Cab arr_sd[10];
	struct Cab arr_hb[10];
	struct Cab arr_su[10];
	struct Cab arr_lx[10];

	FILE *in_sedans, *in_hatchbacks, *in_SUVs, *in_Luxury;
	//open all the 4 text files
	in_sedans = fopen("sedans.txt","r");
	in_hatchbacks = fopen("hatchbacks.txt","r");
	in_SUVs = fopen("SUVs.txt","r");
	in_Luxury = fopen("Luxury.txt","r");
	//read data from all text files into their respective arrays
	scan_data(arr_sd, *in_sedans);
	scan_data(arr_hb, *in_hatchbacks);
	scan_data(arr_su, *in_SUVs);
	scan_data(arr_lx, *in_Luxury);
	//close the input files
	fclose(in_sedans);
	fclose(in_hatchbacks);
	fclose(in_SUVs);
	fclose(in_Luxury);



	gotoxy(30,2);
	printf("==================== CAB MANAGEMENT SYSTEM ====================");
	gotoxy(28,3);
	printf("( PRESENTED BY Ritesh Dabas(2K20/A7/50) and Riya Jain(2K20/A7/53) )");

	gotoxy(10,5);
	printf("Do you want to login as a customer [Y/N]? ");
	char input;
	scanf("%c", &input);

	if (input == 'Y' || input == 'y')
	{ //LOGGED IN AS CUSTOMER
		int go = 0, ride_cost = 0;
		struct Customer c;      //initialise person at 0,0
		char f_name[20], l_name[20];
		int c_x, c_y;

		while (go==0)
		{
			gotoxy(10,6);
			printf("Welcome! Enter your full name. (Type exit program to exit) : ");
			scanf("%s %s",&f_name,&l_name);

			if (f_name == "exit" && l_name == "program")
				return 0;
			
			gotoxy(10,8);
			printf("Welcome %s %s! Enter your present coordinates separated by a space - ");
			scanf("%d %d",&c_x, &c_y);
			//update person coordinates from 0,0 to coordinates user entered
			c.x = c_x;
			c.y = c_y;

			gotoxy(10,9);
			printf("Enter your destination coordinates, separated by a space - ");
			int d_x, d_y;
			scanf("%d %d",&d_x, &d_y);
			//Update Customer destination coordinates to user entered destination coordinates
			c.d_x = d_x;
			c.d_y = d_y;

			bool check = true; //flag is there as a check to see if the user input is valid or not
			while (check)
			{	
				char category[10];
				gotoxy(5,11);
				printf("Choose the category of the cab you want to ride in :");
				gotoxy(5,12);
				printf("Pricing model");
				gotoxy(5,13);
				printf("1. Sedan      base distance = 5 KM    base cost = Rs. 80      Cost per km = Rs.15 ");
				gotoxy(5,14);
				printf("2. SUV        base distance = 10 KM   base cost = Rs. 100     Cost per km = Rs.13 ");
				gotoxy(5,15);
				printf("3. Hatchback  base distance = 8 KM    base cost = Rs. 125     Cost per km = Rs.18 ");
				gotoxy(5,16);
				printf("4. Luxury     base distance = 15 KM   base cost = Rs. 175     Cost per km = Rs.20 ");
				gotoxy(5,18);
				scanf("%s", &category);
				//call journey function according to chosen category and store the cost of ride
				if (strcmp(category,"sedan")==0)
				{
				  ride_cost = journey(arr_sd, c);
				  check = false;
				}
				else if (strcmp(category,"hatchback")==0)
				{
				  ride_cost = journey(arr_hb, c);
				  check = false;
				}
				else if (strcmp(category,"SUV")==0 || strcmp(category,"suv")==0)
				{
				  ride_cost = journey(arr_su, c);
				  check = false;
				}
				else if (strcmp(category,"luxury")==0)
				{
				  ride_cost = journey(arr_lx, c);
				  check = false;
				}
				else
					printf("Oops! Try again! ");
			}

			go = 1;
		}

		char inp;
		gotoxy(5,5);
		printf("Do you have a reference code to avail a 10 percent discount [Y/N]: ");
		scanf(" %c", &inp);

		double discount = 0.1*ride_cost;	// discount is 10% of total cost

		if(inp == 'Y' || inp == 'y'){
			gotoxy(5,6);
			printf("Enter the 6 character reference code : ");
			char ref[7];
			scanf("%s",&ref);

			if(isValid(6,ref)==true){	//if valid, 10% discount is applied, else amount is as is
				gotoxy(5,8);
				printf("Congratulations! Your reference code is valid and you avail a discount of %lf. Your effective fair is %.2lf ", discount, (double)ride_cost-discount);
			}
			else{
				gotoxy(5,8);
				printf("Your reference code is invalid. Your effective fair is %d", ride_cost);
			}
		}
		else{
			gotoxy(5,6);
			printf("Effective Fair is %d ",ride_cost);
		}

		gotoxy(20,10);
		printf("==================== Thanks for riding! Hope you had a great journey! ====================");
	} //Customer Login Ends

	else
    { //LOGGED IN AS TAXI DRIVER
	    struct Cab car_owned;
	    int amount=0; // total money earned
	    
	    gotoxy(5,7);
	    printf("Enter your category (sedans/hatchbacks/SUVs/Luxury) :");
	    char categ[10];
	    scanf("%s", &categ);
	    
	    gotoxy(5,8);
	    printf("Enter your login key: "); //each driver has a unique login key, which is equal to their car's index in their respective array
	    int key;
	    scanf("%d", &key);
	    
	    gotoxy(5,9);
	    printf("Enter your License No: ");
	    int lic_check;
	    scanf("%d", &lic_check);
	    //for each driver, login key serves as username whereas their license number serves as password
	    //check to see if username and password match:
	    if (strcmp(categ,"sedans")==0)
	    {
			if (lic_check == arr_sd[key].license)
			{
				car_owned = arr_sd[key]; //Assign car_owned as the car that user logged in as
				gotoxy(15,11);
				printf("================= Sucessful Login as a driver ================= ");
				gotoxy(5,12);
				printf("Welcome %s!", arr_sd[key].name);
				gotoxy(5,13);
				printf("Your rating is %lf", arr_sd[key].rating);
			}
			else
			{
				gotoxy(15,11);
				printf("================= ERROR. WRONG DETAILS =================");
				return 1;
			}
	    }
	    //similar code for other categories:
	    else if (strcmp(categ,"hatchbacks")==0)
	    {
			if (lic_check == arr_hb[key].license)
			{
				car_owned = arr_hb[key]; 
				gotoxy(15,11);
				printf("================= Sucessful Login as a driver ================= ");
				gotoxy(5,12);
				printf("Welcome %s!", arr_hb[key].name);
				gotoxy(5,13);
				printf("Your rating is %lf", arr_hb[key].rating);
			}
			else
			{
				gotoxy(15,11);
				printf("================= ERROR. WRONG DETAILS =================");
				return 1;
			}
	    }
	    else if (strcmp(categ,"SUVs")==0)
	    {
			if (lic_check == arr_su[key].license)
			{
				car_owned = arr_su[key]; 
				gotoxy(15,11);
				printf("================= Sucessful Login as a driver ================= ");
				gotoxy(5,12);
				printf("Welcome %s!", arr_su[key].name);
				gotoxy(5,13);
				printf("Your rating is %lf", arr_su[key].rating);
			}
			else
			{
				gotoxy(15,11);
				printf("================= ERROR. WRONG DETAILS =================");
				return 1;
			}
	    }
	    else if (strcmp(categ,"luxury")==0)
	    {
			if (lic_check == arr_lx[key].license)
			{
				car_owned = arr_lx[key]; 
				gotoxy(15,11);
				printf("================= Sucessful Login as a driver ================= ");
				gotoxy(5,12);	
				printf("Welcome %s!", arr_lx[key].name);
				gotoxy(5,13);
				printf("Your rating is %lf", arr_lx[key].rating);
			}
			else
			{
				gotoxy(15,11);
				printf("================= ERROR. WRONG DETAILS =================");
				return 1;
			}
	    }

	    while (true)		// infinite loop that runs till user says they want to end their day asked after each ride
	    {                        
			Sleep(5000);
			clrscr();
			gotoxy(5,2);
			char a;
			printf("Would you like to end your day? [Y/N] "); 
			scanf(" %c", &a);

			if (a == 'Y' || a == 'y')
			{	
				gotoxy(5,4);
				printf("You earned Rs.%d today. Goodbye!", amount);
				break;
			}

			else
			{
				int x, y, dx, dy;
				//Assign passenger with random coordinates to Driver
				srand(time(0));
				x = rand() % 50;
				dx = rand() % 50;
				y = rand() % 50;
				dy = rand() % 50;
				//Create a Customer object
				struct Customer c;
				Customer(c,x, y, dx, dy);
				gotoxy(5,4);
				printf("Your passenger is at %d,%d and their destination is %d,%d.", x, y, dx , dy);
				gotoxy(5,5);
				printf("Go to your passenger and drive them to their destination");
				Sleep(5000);
				gotoxy(15,7);
				printf("Your ride is over. You earned Rs.%d !", get_cost(c, car_owned));
				
				double rat;
				amount += get_cost(c, car_owned); //Add money earned to total_money
				if (strcmp(categ,"sedans")==0)
				  rat = driver_rate(arr_sd[key]); 	//updates car rating and assigns rat as whatever value they were rated
				else if (strcmp(categ,"hatchbacks")==0)
				  rat = driver_rate(arr_hb[key]);
				else if (strcmp(categ,"SUVs")==0)
				  rat = driver_rate(arr_su[key]);
				else if(strcmp(categ,"luxury")==0)
				  rat = driver_rate(arr_lx[key]);

				gotoxy(15,8);
				printf("You were rated %lf", rat); 		//shows the driver what they were rated
			}
	    }
    }



	FILE *out_hatchbacks, *out_sedans, *out_Luxury, *out_SUVs;
	//open the text files
	out_hatchbacks = fopen("hatchbacks.txt","w");
	out_SUVs = fopen("SUVs.txt","w");
	out_Luxury = fopen("Luxury.txt","w");
	out_sedans = fopen("sedans.txt","w");
	//output the updated data of all the cars into their respective txt fies:
	print_data(arr_sd, *out_sedans);
	print_data(arr_hb, *out_hatchbacks);
	print_data(arr_su, *out_SUVs);
	print_data(arr_lx, *out_Luxury);
	//close all the output streams:
	fclose(out_sedans);
	fclose(out_hatchbacks);
	fclose(out_SUVs);
	fclose(out_Luxury);

	return 0;

}
