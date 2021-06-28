#include <stdio.h>
#include<string.h>

//default size to initialize databases
#define char_size 50
#define max_books 100
#define max_members 100

//today's date
#define cur_day 1
#define cur_month 12
#define cur_year 2020

//location of the book in the library
struct book_loc
{
	int hall_num;
	int row ;
	int rack;
};

//database of all the books from the library
struct book_DB
{
	char bk_name[char_size];
	char bk_subject[char_size];
	char bk_author[char_size][char_size]; //in case of multiple authors
	int acc_num;
	int publish_yr;
	int status; //0:issued, 1:available
	struct book_loc loc;
};

//database of all the members present in the campus
struct date
{
	int day;
	int month;
	int year;
};

//details for the books issued
struct issue_detail
{
	int member_id;
	int acc_num;
	struct date issue_dt;
	struct date due_dt;
	int renew_num;
};

//database of all the members present in the campus
struct member_DB
{
	char name[char_size];
	char dept[char_size];
	int design; //0: student, 1: faculty
	int member_id;
	int issue_bk_num;
};

//User Defined Functions

int menu(struct book_DB BK[], struct member_DB MEM[],
	struct issue_detail ID[], int BK_num, 
	int MEM_num, int ID_num, int call_num);

int init(struct book_DB BK[], int BK_size,
	struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size);

void add_bk(struct book_DB BK[], int BK_size,
	struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size);

void add_mem(struct member_DB MEM[], int MEM_size);

void max_issue_mem(struct member_DB MEM[], int MEM_size);

void fine_calc(struct member_DB MEM[], int MEM_size,
	struct issue_detail ID[], int ID_size);

void display_br(struct member_DB MEM[], int MEM_size);

void available(struct book_DB BK[], int BK_size);

int issue(struct book_DB BK[], int BK_size,
	struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size);

void renew(struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size);

//main funtion; will initialise stucture variables
main()
{
	struct book_DB BK[max_books];
	struct member_DB MEM[max_members];
	struct issue_detail ID[max_members];
	int BK_num=0, MEM_num=0, ID_num=0, call_num=0;
	menu(BK,MEM,ID,BK_num,MEM_num,ID_num,call_num);
}

//the menu function will display all the options we can chose from
int menu(struct book_DB BK[], struct member_DB MEM[],
	struct issue_detail ID[], int BK_num,
	int MEM_num, int ID_num, int call_num)
{
	//for the first call of the function, the databases will be initialised with pre-existing data
	if(call_num==0)
	{
		printf("Enter the number of books presently in the library (issued and available): ");
		scanf("%d", &BK_num);

		printf("Enter the number of members presently in the campus (student and faculty): ");
		scanf("%d", &MEM_num);

		ID_num = init(BK, BK_num, ID, ID_num, MEM, MEM_num);
	}

	//displaying the main menu
	int x;
	printf("\tMain Menu\n");
	printf("\n1. Insert a book");
	printf("\n2. Add a member");
	printf("\n3. Find member with maximum issue");
	printf("\n4. Calculate fine");
	printf("\n5. Display borrowers");
	printf("\n6. Check availability of a book");
	printf("\n7. Issue a book");
	printf("\n8. Renew issue\n");
	scanf("%d", &x);
	//users can enter a number to perform the corresponding activity

	switch(x)
	{
		case 1: BK_num++;
				add_bk(BK, BK_num, ID, ID_num, MEM, MEM_num); //func to add a new book into the databse
				break;

		case 2: MEM_num++;
				add_mem(MEM, MEM_num); //func to add a new member into the database
				break;

		case 3: max_issue_mem(MEM, MEM_num); //will display maxmimum book borrowers (for faculty and student seperately)
				break;

		case 4: fine_calc(MEM, MEM_num, ID, ID_num); //will calulate fine for every member and display maximum fine
				break;

		case 5: display_br(MEM, MEM_num); //will sort the display the members' list
				break;

		case 6: available(BK, BK_num); //will check for availibility of a book
				break;

		case 7: ID_num += issue(BK, BK_num, ID, ID_num, MEM, MEM_num); //will issue a book
				break;

		case 8: renew(ID, ID_num, MEM, MEM_num); //will renew the due date
				break;

		default:printf("INVALID INPUT. Please try again\n");
				call_num++;
                menu(BK,MEM,ID,BK_num,MEM_num,ID_num,call_num);
	}

	char ch;
	printf("\n\nDo you wish to continue? (y/n)\t"); //for rerunning the program for some other purposes
	scanf("%s", &ch);
	if(ch=='y')
    {
    	call_num++;
        menu(BK,MEM,ID,BK_num,MEM_num,ID_num,call_num);
    }

    return 0;
}

int init(struct book_DB BK[], int BK_size,
	struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size)
{
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int i, j, k, days_dif, insert_pos, flag, borrow_design, author_num;
	char subject[char_size], ch;

 	//collecting the members' list
	for(i=0; i<MEM_size; i++)
	{
		printf("\nEntering Member details for member: %d\n", i+1);

        getchar();
		printf("Enter MEMBER NAME: ");
		gets(MEM[i].name);

		printf("Enter DEPARTMENT: ");
		gets(MEM[i].dept);

		printf("Enter DESIGNATION (0: student, 1: faculty): ");
		scanf("%d", &MEM[i].design);

		printf("Enter MEMBER ID: ");
		scanf("%d", &MEM[i].member_id);

		MEM[i].issue_bk_num = 0;
	}

	//collecting the books' list, sorting it subject-wise
	for(i=0; i<BK_size; i++)
	{
		printf("\nEntering book details\n");

		getchar();
		printf("Enter BOOK SUBJECT: ");
		gets(subject);

		insert_pos=i;
		flag=0;

		for(j=0; j<i && flag==0; j++)
		{
			k=j;
			while(strcmp(subject, BK[k].bk_subject)==0)
			{
				k++;
				flag = 1;
			}
			if(flag==1)
			{
				insert_pos = k;
			}
		}

		j=i;
		while(j>insert_pos)
		{
			BK[j] = BK[j-1];
			j--;
		}

		strcpy(BK[insert_pos].bk_subject,subject);

		printf("Enter BOOK NAME: ");
		gets(BK[insert_pos].bk_name);

		ch='y';
		author_num = 0;
		while(ch=='y')
		{
			printf("Enter BOOK AUTHOR NAME: ");
	   		gets(BK[insert_pos].bk_author[author_num]);

	   		printf("Are there more authors? (y/n)\t");
	   		scanf("%s", &ch);

	   		author_num++;
	   		getchar();
		}

		printf("Enter BOOK ACCESSION NUMBER: ");
		scanf("%d", &BK[insert_pos].acc_num);

		printf("Enter BOOK PUBLISH YEAR: ");
		scanf("%d", &BK[insert_pos].publish_yr);

		printf("Enter BOOK STATUS (0: Issued, 1: Available): ");
		scanf("%d", &BK[insert_pos].status);

		if(BK[insert_pos].status==0)
		{
			flag=0;
			while(flag==0)
			{
				printf("Enter BORROWER MEMBER ID: ");
				scanf("%d", &ID[ID_size].member_id);

				for(j=0; j<MEM_size; j++)
				{
					if(MEM[j].member_id==ID[ID_size].member_id)
					{
						flag=1;
						borrow_design = MEM[j].design;
						MEM[j].issue_bk_num++;
					}
				}

				if(flag==0)
				{
					printf("MEMBER ID does not exist. Please enter valid MEMBER ID\n");
				}
			}


			ID[ID_size].acc_num = BK[insert_pos].acc_num;
			ID[ID_size].renew_num = 0;

			printf("Enter ISSUE DATE dd{space}mm{space}yyyy: ");
			scanf("%d%d%d", &ID[ID_size].issue_dt.day,
				&ID[ID_size].issue_dt.month,
				&ID[ID_size].issue_dt.year);

			if(borrow_design==0)
			{
				days_dif = ID[ID_size].issue_dt.day + 14 - month_days[ID[ID_size].issue_dt.month-1];
				if(days_dif<=0)
				{
					ID[ID_size].due_dt.day = ID[ID_size].issue_dt.day + 14;
					ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month;
					ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
				}
				else
				{
					ID[ID_size].due_dt.day = days_dif;
					if(ID[ID_size].issue_dt.month!=12)
					{
						ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month+1;
						ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
					}
					else
					{
						ID[ID_size].due_dt.month = 1;
						ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year+1;
					}
				}
			}
			else
			{
				ID[ID_size].due_dt.day = ID[ID_size].issue_dt.day;
				if(ID[ID_size].issue_dt.month!=12)
				{
					ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month+1;
					ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
				}
				else
				{
					ID[ID_size].due_dt.month = 1;
					ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year+1;
				}
			}

			ID_size++;
		}
		else
		{
			printf("Enter HALL NUMBER: ");
			scanf("%d", &BK[insert_pos].loc.hall_num);

			printf("Enter RACK NUMBER: ");
			scanf("%d", &BK[insert_pos].loc.rack);

			printf("Enter ROW NUMBER: ");
			scanf("%d", &BK[insert_pos].loc.row);
		}
	}

	return ID_size;
}

void add_bk(struct book_DB BK[], int BK_size,
	struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size)
{
	char subject[char_size], ch, author_num;
	int i,j, insert_pos,flag;

	printf("\nEntering book details\n");

	getchar();
	printf("Enter BOOK SUBJECT: ");
	gets(subject);

	insert_pos=BK_size-1;
	flag=0;

	for(i=0; i<BK_size-1 && flag==0; i++)
	{
		j=i;
		while(strcmp(subject, BK[j].bk_subject)==0)
		{
			j++;
			flag = 1;
		}
		if(flag==1)
		{
			insert_pos = j;
		}
	}

	i=BK_size-1;
	while(i>insert_pos)
	{
		BK[i] = BK[i-1];
		i--;
	}

	strcpy(BK[insert_pos].bk_subject,subject);

	printf("Enter BOOK NAME: ");
	gets(BK[insert_pos].bk_name);

	ch='y';
	author_num = 0;
	while(ch=='y')
	{
        getchar();

		printf("Enter BOOK AUTHOR NAME: ");
	   	gets(BK[insert_pos].bk_author[author_num]);

	   	printf("Are there more authors? (y/n)\t");
	   	scanf("%c", &ch);

	   	author_num++;
	}

	printf("Enter BOOK ACCESSION NUMBER: ");
	scanf("%d", &BK[insert_pos].acc_num);

	printf("Enter BOOK PUBLISH YEAR: ");
	scanf("%d", &BK[insert_pos].publish_yr);

	BK[insert_pos].status = 1;

	printf("Enter BOOK PUBLISH YEAR: ");
	scanf("%d", &BK[insert_pos].publish_yr);

	printf("\nEntering BOOK LOCATION\n");

	printf("Enter HALL NUMBER: ");
	scanf("%d", &BK[insert_pos].loc.hall_num);

	printf("Enter RACK NUMBER: ");
	scanf("%d", &BK[insert_pos].loc.rack);

	printf("Enter ROW NUMBER: ");
	scanf("%d", &BK[insert_pos].loc.row);
}

void add_mem(struct member_DB MEM[], int MEM_size)
{
	printf("\nEntering Member details\n");

	getchar();
	printf("Enter MEMBER NAME: ");
	gets(MEM[MEM_size-1].name);

	printf("Enter DEPARTMENT: ");
	gets(MEM[MEM_size-1].dept);

	printf("Enter DESIGNATION (0: student, 1: faculty): ");
	scanf("%d", &MEM[MEM_size-1].design);

	printf("Enter MEMBER ID: ");
	scanf("%d", &MEM[MEM_size-1].member_id);

	MEM[MEM_size-1].issue_bk_num = 0;
}

void max_issue_mem(struct member_DB MEM[], int MEM_size)
{
	int student_max_issue, faculty_max_issue, max_student_index[max_members],
		max_faculty_index[max_members], i,j,k;

	student_max_issue = 0;
	for(i=0; i<MEM_size; i++)
	{
		if(MEM[i].design==0)
		{
			if(MEM[i].issue_bk_num>student_max_issue)
			{
				student_max_issue = MEM[i].issue_bk_num;
				j=0;
				max_student_index[j] = i;
			}
			else if(MEM[i].issue_bk_num==student_max_issue)
			{
				j++;
				max_student_index[j] = i;
			}
		}
	}

	faculty_max_issue = 0;
	for(i=0; i<MEM_size; i++)
	{
		if(MEM[i].design==1)
		{
			if(MEM[i].issue_bk_num>faculty_max_issue)
			{
				faculty_max_issue = MEM[i].issue_bk_num;
				k=0;
				max_faculty_index[k] = i;
			}
			else if(MEM[i].issue_bk_num==faculty_max_issue)
			{
				k++;
				max_faculty_index[k] = i;
			}
		}
	}

	printf("\n\nMAX ISSUE\n");
	printf("\nStudent:\nNumber of ISSUES: %d\nNames: ", student_max_issue);
	for(i=0; i<=j; i++)
	{
		printf("%s\n", MEM[max_student_index[i]].name);
	}

	printf("\nFaculty:\nNumber of ISSUES: %d\nNames: ", faculty_max_issue);
	for(i=0; i<=k; i++)
	{
		printf("%s\n", MEM[max_faculty_index[i]].name);
	}
}

void fine_calc(struct member_DB MEM[], int MEM_size,
    struct issue_detail ID[], int ID_size)
{
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int i, j, k, fine_days, due_days, tot_days, flag,
		fines[max_members] = {0}, max_fine, defaulters[max_members];

	tot_days = cur_day;
	for(i=0; i<cur_month-1; i++)
	{
		tot_days += month_days[i];
	}

	for(i=0; i<ID_size; i++)
	{
		due_days = ID[i].due_dt.day;
		for(j=0; j<ID[i].due_dt.month-1; j++)
		{
			due_days += month_days[j];
		}

		fine_days = due_days-tot_days;   //assuming all books were borrowed in 2020 (present year)

		if(fine_days<0)
		{
			k=0;
			max_fine = 0;
			flag = 0;

			for(j=0; j<MEM_size && flag==0; j++)
			{
				if(ID[i].member_id==MEM[j].member_id)
				{
					fines[j] += 0.5 * fine_days * (-1);
					flag=1;

					if(max_fine<fines[j])
					{
						max_fine = fines[j];
						k=0;
						defaulters[k] = j;
					}
					else if(max_fine==fines[j])
					{
						k++;
						defaulters[k] = j;
					}
				}
			}
		}
	}

	printf("\nMAX FINES RECORD\n\n");
	for(i=0; i<=k; i++)
	{
		printf("MEMBER NAME: %s\n", MEM[defaulters[i]].name);
		printf("DEPARTMENT NAME: %s\n", MEM[defaulters[i]].dept);
		printf("DESIGNATION (0: student, 1: faculty): %d\n", MEM[defaulters[i]].design);
		printf("MEMBER ID: %d\n", MEM[defaulters[i]].member_id);
		printf("NUMBER OF BOOKS ISSUED: %d\n", MEM[i].issue_bk_num);
		printf("FINE: Rs. %d\n", max_fine);
		printf("\n");
	}
}

void display_br(struct member_DB MEM[], int MEM_size)
{
	int flag, i, j;
	struct member_DB temp;

	flag = 1;
	for(i=0; i<MEM_size && flag==1; i++)
	{
		flag = 0;
		for(j=0; j<MEM_size-1-i; j++)
		{
			if(MEM[j].issue_bk_num<MEM[j+1].issue_bk_num)
			{
				temp = MEM[j];
				MEM[j] = MEM[j+1];
				MEM[j+1] = temp;
				flag=1;
			}

			else if(MEM[j].issue_bk_num==MEM[j+1].issue_bk_num)
			{
				if(MEM[j].design==0 && MEM[j+1].design==1)
				{
					temp = MEM[j];
					MEM[j] = MEM[j+1];
					MEM[j+1] = temp;
					flag=1;
				}
				else if(MEM[j].design==MEM[j+1].design)
				{
					if(strcmp(MEM[j].name,MEM[j+1].name)<0)
					{
						temp = MEM[j];
						MEM[j] = MEM[j+1];
						MEM[j+1] = temp;
						flag=1;
					}
				}
			}
		}
	}

	for(i=0; i<MEM_size; i++)
	{
		printf("MEMBER NAME: %s\n", MEM[i].name);
		printf("DEPARTMENT NAME: %s\n", MEM[i].dept);
		printf("DESIGNATION (0: student, 1: faculty): %d\n", MEM[i].design);
		printf("MEMBER ID: %d\n", MEM[i].member_id);
		printf("NUMBER OF BOOKS ISSUED: %d\n", MEM[i].issue_bk_num);
		printf("\n");
	}
}

void available(struct book_DB BK[], int BK_size)
{
	char title_name[char_size];
	int i, flag;

	getchar();
	printf("Enter the name of the book: ");
	gets(title_name);

	flag=0;
	for(i=0; i<BK_size && flag==0; i++)
	{
		if(strcmp(BK[i].bk_name,title_name)==0)
		{
			if(BK[i].status==1)
			{
				printf("The book is available\n");
				printf("BOOK NAME: %s\n", BK[i].bk_name);
				printf("BOOK SUBJECT: %s\n", BK[i].bk_subject);
				printf("BOOK ACCESSION NUMBER %d\n", BK[i].acc_num);
				printf("PUBLSIH YEAR: %d\n", BK[i].publish_yr);
				printf("BOOK AVAILABILITY STATUS: %d\n", BK[i].status);
				flag=1;
			}
		}
	}

	if(flag==0)
	{
		printf("The book is unavailable\n");
	}
}

int issue(struct book_DB BK[], int BK_size,
	struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size)
{
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int flag, member_id,hall_num, rack, row, i, mem_index, flag1, days_dif;

	printf("Enter your member id: ");
	scanf("%d", &member_id);

	flag==0;
	for(i=0; i<MEM_size && flag==0; i++)
	{
		if(MEM[i].member_id==member_id)
		{
			flag=1;
			mem_index = i;
			if(MEM[i].design==0 && MEM[i].issue_bk_num>2)
			{
				flag=2;
			}
			else if(MEM[i].design==1 && MEM[i].issue_bk_num>4)
			{
				flag=2;
			}
		}
	}

	if(flag==1)
	{
		printf("Enter the location of the book (hall no., rack, row): ");
		scanf("%d%d%d", &hall_num, &rack, &row);

		flag1=0;
		for(i=0; i< BK_size && flag1==0; i++)
		{
			if(BK[i].status==1)
			{
				if(BK[i].loc.hall_num==hall_num && BK[i].loc.rack==rack
					&& BK[i].loc.row==row)
				{
					printf("\nBOOK NAME: %s", BK[i].bk_name);
					printf("\nSUBJECT: %s", BK[i].bk_subject);
					ID[ID_size].acc_num = BK[i].acc_num;
					ID[ID_size].member_id = MEM[mem_index].member_id;
					ID[ID_size].renew_num = 0;
					MEM[mem_index].issue_bk_num++;

					ID[ID_size].issue_dt.day = cur_day;
					ID[ID_size].issue_dt.month = cur_month;
					ID[ID_size].issue_dt.year = cur_year;

					if(MEM[mem_index].design==0)
					{
						days_dif = ID[ID_size].issue_dt.day + 14 -
							month_days[ID[ID_size].issue_dt.month-1];

						if(days_dif<=0)
						{
							ID[ID_size].due_dt.day = ID[ID_size].issue_dt.day + 14;
							ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month;
							ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
						}
						else
						{
							ID[ID_size].due_dt.day = days_dif;
							if(ID[ID_size].issue_dt.month!=12)
							{
								ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month+1;
								ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
							}
							else
							{
								ID[ID_size].due_dt.month = 1;
								ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year+1;
							}
						}
					}
					else
					{
						ID[ID_size].due_dt.day = ID[ID_size].issue_dt.day;
						if(ID[ID_size].issue_dt.month!=12)
						{
							ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month+1;
							ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
						}
						else
						{
							ID[ID_size].due_dt.month = 1;
							ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year+1;
						}
					}

					ID_size++;

					printf("\nISSUE DATE: %d-%d-%d", ID[i].issue_dt.day,
						ID[i].issue_dt.month, ID[i].issue_dt.year);

					printf("\nDUE DATE: %d-%d-%d", ID[i].due_dt.day,
					ID[i].due_dt.month, ID[i].due_dt.year);

					flag1=1;
				}
			}
		}

		if(flag1==0)
		{
			printf("Book not available\n");
		}
	}
	else if(flag==2)
	{
		printf("You have reached your maximum limit on issuing books\n");
	}
	else
	{
		printf("\nINVALID MEMBER ID\n");
	}

	return ID_size;
}

void renew(struct issue_detail ID[], int ID_size,
	struct member_DB MEM[], int MEM_size)
{
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int days_dif, member_id, acc_num, tot_days, flag, mem_index, i, due_days, fine;

	printf("\nEnter your MEMBER ID and the BOOK ACCESSION NUMBER: ");
	scanf("%d%d", &member_id, &acc_num);

	flag=0;
	for(i=0; i<MEM_size && flag==0; i++)
	{
		if(MEM[i].member_id==member_id)
		{
			flag = 1;
			mem_index = i;
		}
	}

	for(i=0; i<cur_month-1; i++)
	{
		tot_days += month_days[i];
	}

	flag=0;
	for(i=0; i<ID_size && flag==0; i++)
	{
		if(ID[i].renew_num<3)
		{
			if(ID[i].member_id==member_id && ID[i].acc_num==acc_num)
			{
				flag=1;
				due_days = ID[i].due_dt.day;
				for(i=0; i<ID[i].due_dt.month-1; i++)
				{
					due_days += month_days[i];
				}

				if(due_days-tot_days>0)
				{
					ID[i].issue_dt.day = cur_day;
					ID[i].issue_dt.month = cur_month;
					ID[i].issue_dt.year = cur_year;

					if(MEM[mem_index].design==0)
					{
						days_dif = ID[i].issue_dt.day + 14 -
						month_days[ID[i].issue_dt.month-1];

						if(days_dif<=0)
						{
							ID[i].due_dt.day = ID[i].issue_dt.day + 14;
							ID[i].due_dt.month = ID[i].issue_dt.month;
							ID[i].due_dt.year = ID[i].issue_dt.year;
						}
						else
						{
							ID[i].due_dt.day = days_dif;
							if(ID[i].issue_dt.month!=12)
							{
								ID[i].due_dt.month = ID[i].issue_dt.month+1;
								ID[ID_size].due_dt.year = ID[i].issue_dt.year;
							}
							else
							{
								ID[ID_size].due_dt.month = 1;
								ID[i].due_dt.year = ID[i].issue_dt.year+1;
							}
						}
					}
					else
					{
						ID[ID_size].due_dt.day = ID[ID_size].issue_dt.day;
						if(ID[ID_size].issue_dt.month!=12)
						{
							ID[ID_size].due_dt.month = ID[ID_size].issue_dt.month+1;
							ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year;
						}
						else
						{
							ID[ID_size].due_dt.month = 1;
							ID[ID_size].due_dt.year = ID[ID_size].issue_dt.year+1;
						}
					}

					printf("\nThe new due date is %d-%d-%d\n", ID[i].due_dt.day,
						ID[i].due_dt.month, ID[i].due_dt.year);
				}
				else
				{
					fine = (due_days-tot_days) * 0.5;
					printf("The book was due already. Please pay fine of %d\n", fine);
				}
			}
			else
			{
				printf("\nBOOK NOT FOUND\n");
			}
		}
		else
		{
			printf("\nYou have reached maximum renews\n");
		}
	}

	if(flag==0)
	{
		printf("\nInvalid MEMBER ID or BOOK ACCESSION NUMBER\n");
	}
}
