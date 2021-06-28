#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define char_size 50

//today's date
#define cur_day 21
#define cur_month 3
#define cur_year 2021

struct bk_loc
{
	int hall_num;
	int row ;
	int rack;
};

struct date
{
	int day;
	int month;
	int year;
};

struct issue_detail
{
	int member_id;
	struct date issue_dt;
	struct date due_dt;
	int renew_num;
};

struct Bk_Record
{
	char Name[char_size];
	char bk_subject[char_size];
	char bk_author[char_size][char_size]; //in case of multiple authors
	int acc_num;
	int publish_yr;
	int flag; //0:issued, 1:available
	struct bk_loc loc;
	struct issue_detail issue;
	struct Bk_Record* next;
};

struct Mem_Record
{
	char name[char_size];
	char dept[char_size];
	int design; //0: Student, 1: faculty
	int member_id;
	int issue_bk_num;
	struct Mem_Record* next;
};

int menu(struct Bk_Record** bk_ptr, 
	struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr);

void init(struct Bk_Record** bk_ptr, 
	struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr);

struct Mem_Record* mem_input(struct Mem_Record* new_rec, int mem_desig);

struct Bk_Record* bk_input(struct Bk_Record* new_rec, struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr);

void add_bk(struct Bk_Record** bk_ptr, 
	struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr);

void add_mem(struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr);

void max_issue_mem(struct Mem_Record* fac_list, 
	struct Mem_Record* stud_list);

void display_mem(struct Mem_Record* mem_rec, int key);

void sort_display(struct Mem_Record* fac_list, struct Mem_Record* stud_list);

int sort(struct Mem_Record** mem_list);

void chk_available(struct Bk_Record* bk_list);

void bk_issue(struct Bk_Record** bk_ptr, 
	struct Mem_Record** mem_ptr);

void bk_renew(struct Bk_Record** bk_ptr, 
	struct Mem_Record* mem_ptr);

main()
{
	struct Bk_Record* book_record = NULL;
	struct Mem_Record* faculty_record = NULL;
	struct Mem_Record* student_record = NULL;
	
	menu(&book_record, &faculty_record, &student_record);
}

int menu(struct Bk_Record** bk_ptr, 
	struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr)
{
	static int call_num = 0;
	int design;
	struct Bk_Record* bk_list = (*bk_ptr);
	struct Mem_Record* fac_list = (*fac_ptr);
	struct Mem_Record* stud_list = (*stud_ptr);

	if(call_num==0)
	{	
		init(&bk_list, &fac_list, &stud_list);
		call_num++;
	}

	//displaying the main menu
	int x;
	printf("\t\nMain Menu\n");
	printf("\n1. Insert a book");
	printf("\n2. Add a member");
	printf("\n3. Find member with maximum issue");
	printf("\n4. Display borrowers");
	printf("\n5. Check availability of a book");
	printf("\n6. Issue a book");
	printf("\n7. Renew issue\n");
	scanf("%d", &x);
	//users can enter a number to perform the corresponding activity

	switch(x)
	{
		case 1: add_bk(&bk_list, &fac_list, &stud_list); //func to add a new book into the databse
				break;

		case 2: add_mem(&fac_list, &stud_list); //func to add a new member into the database
				break;

		case 3: max_issue_mem(fac_list, stud_list); //will display maxmimum book borrowers (for faculty and student seperately)
				break;

		case 4: sort_display(fac_list, stud_list); //will sort the display the members' list
				break;

		case 5: chk_available(bk_list); //will check for availibility of a book
				break;

		case 6: printf("Enter DESIGNATION (0: student, 1: faculty): ");
				scanf("%d", &design);

				if(design==0)
				{
					bk_issue(&bk_list, &stud_list); //will issue a book
				}
				else
				{
					bk_issue(&bk_list, &fac_list);	
				}

				break;

		case 7: printf("Enter DESIGNATION (0: student, 1: faculty): ");
				scanf("%d", &design);

				if(design==0)
				{
					bk_renew(&bk_list, stud_list); //will renew the due date
				}
				else
				{
					bk_renew(&bk_list, fac_list);	
				}

				break;

		default: printf("INVALID INPUT. Please try again\n");
	}

	menu(&bk_list, &fac_list, &stud_list);
	return 0;
}

void init(struct Bk_Record** bk_ptr, 
	struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr)
{
	struct Bk_Record* temp_bk = NULL;
	struct Bk_Record* new_bk = NULL;
	
	struct Mem_Record* temp_fac = (*fac_ptr);
	struct Mem_Record* new_fac = NULL;
	struct Mem_Record* temp_stud = (*stud_ptr);
	struct Mem_Record* new_stud = NULL;
	
	char ch;
	int mem_type, status;

	ch = 'y';
	printf("\nInitialsing Member Database\n\n");
	while(ch=='y')
	{
		printf("Enter DESIGNATION (0: student, 1: faculty): ");
		scanf("%d", &mem_type);

		if(mem_type==0)
		{
			new_stud = (struct Mem_Record*)malloc(sizeof(struct Mem_Record));
			new_stud = mem_input(new_stud, mem_type);
			
			if(temp_stud==NULL)
			{
				(*stud_ptr) = temp_stud = new_stud;
			}
			else
			{
				temp_stud->next = new_stud;
				temp_stud = temp_stud->next;
			}
		}
		else
		{
			new_fac = (struct Mem_Record*)malloc(sizeof(struct Mem_Record));
			new_fac = mem_input(new_fac, mem_type);

			if(temp_fac==NULL)
			{
				(*fac_ptr) = temp_fac = new_fac;
			}
			else
			{
				temp_fac->next = new_fac;
				temp_fac = temp_fac->next;
			}
		}

		printf("Are there more entries?(y/n): ");
		scanf("%s", &ch);
	}
	
	ch = 'y';
	printf("\nInitialsing Book Database\n\n");
	while(ch=='y')
	{
		temp_fac = (*fac_ptr);
		temp_stud = (*stud_ptr);

		new_bk = (struct Bk_Record*)malloc(sizeof(struct Bk_Record));
		new_bk = bk_input(new_bk, &temp_fac, &temp_stud);
		
		if(temp_bk==NULL)
		{
			temp_bk = (*bk_ptr) = new_bk;
		}
		else
		{
			temp_bk = (*bk_ptr);
			status = 0;

			while(temp_bk->next!=NULL && status==0)
			{
				if(strcmp(new_bk->bk_subject, temp_bk->bk_subject)==0)
				{
					status = 1;
					new_bk->next = temp_bk->next;
					temp_bk->next = new_bk;
				}

				temp_bk = temp_bk->next;
			}

			if(status==0)
			{
				new_bk->next = temp_bk->next;
				temp_bk->next = new_bk;
			}
		}

		printf("Are there more records?(y/n) : ");
		scanf("%s", &ch);
	}
}

struct Mem_Record* mem_input(struct Mem_Record* new_rec, int mem_desig)
{
	new_rec->design = mem_desig;
	printf("Entering MEMBER Details\n");

	getchar();
	printf("Enter Member NAME: ");
	gets(new_rec->name);

	printf("Enter DEPARTMENT: ");
	gets(new_rec->dept);

	printf("Enter MEMBER ID: ");
	scanf("%d", &new_rec->member_id);

	new_rec->issue_bk_num = 0;
	new_rec->next = NULL;

	return new_rec;
}

struct Bk_Record* bk_input(struct Bk_Record* new_rec, struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr)
{
	char ath_ch='y';
	int author_num = 0, status, days_dif;
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	struct Mem_Record* temp_fac = NULL;
	struct Mem_Record* temp_stud = NULL;

	getchar();
	printf("Enter BOOK SUBJECT: ");
	gets(new_rec->bk_subject);

	printf("Enter BOOK NAME: ");
	gets(new_rec->Name);

	while(ath_ch=='y')
	{
		printf("Enter BOOK AUTHOR NAME: ");
	   	gets(new_rec->bk_author[author_num]);

	   	printf("Are there more authors?(y/n): ");
	   	scanf("%s", &ath_ch);

	   	author_num++;
	   	getchar();
	}

	printf("Enter BOOK ACCESSION NUMBER: ");
	scanf("%d", &new_rec->acc_num);

	printf("Enter BOOK PUBLISH YEAR: ");
	scanf("%d", &new_rec->publish_yr);

	printf("Enter BOOK STATUS (0: Issued, 1: Available): ");
	scanf("%d", &new_rec->flag);

	if(new_rec->flag==0)
	{
		status = 0;
		while(status==0)
		{
			printf("Enter BORROWER MEMBER ID: ");
			scanf("%d", &new_rec->issue.member_id);

			temp_fac = (*fac_ptr);
			temp_stud = (*stud_ptr);

			while((temp_fac!=NULL || temp_stud!=NULL) && status==0)
			{
				if(temp_fac!=NULL)
				{
					if(temp_fac->member_id==new_rec->issue.member_id)
					{
						status = 1;
						temp_fac->issue_bk_num++;

						printf("Enter ISSUE DATE dd{space}mm{space}yyyy: ");
						scanf("%d%d%d", &new_rec->issue.issue_dt.day,
							&new_rec->issue.issue_dt.month,
							&new_rec->issue.issue_dt.year);

						new_rec->issue.due_dt.day = new_rec->issue.issue_dt.day;
						if(new_rec->issue.issue_dt.month!=12)
						{
							new_rec->issue.due_dt.month = new_rec->issue.issue_dt.month+1;
							new_rec->issue.due_dt.year = new_rec->issue.issue_dt.year;
						}
						else
						{
							new_rec->issue.due_dt.month = 1;
							new_rec->issue.due_dt.year = new_rec->issue.issue_dt.year+1;
						}
					}

					temp_fac = temp_fac->next;
				}

				if(temp_stud!=NULL)
				{
					if(temp_stud->member_id==new_rec->issue.member_id)
					{
						status = 1;
						temp_stud->issue_bk_num++;

						printf("Enter ISSUE DATE dd{space}mm{space}yyyy: ");
						scanf("%d%d%d", &new_rec->issue.issue_dt.day,
							&new_rec->issue.issue_dt.month,
							&new_rec->issue.issue_dt.year);

						days_dif = new_rec->issue.issue_dt.day + 14 - month_days[new_rec->issue.issue_dt.month-1];
						if(days_dif<=0)
						{
							new_rec->issue.due_dt.day = new_rec->issue.issue_dt.day + 14;
							new_rec->issue.due_dt.month = new_rec->issue.issue_dt.month;
							new_rec->issue.due_dt.year = new_rec->issue.issue_dt.year;
						}
						else
						{
							new_rec->issue.due_dt.day = days_dif;
							if(new_rec->issue.issue_dt.month!=12)
							{
								new_rec->issue.due_dt.month = new_rec->issue.issue_dt.month+1;
								new_rec->issue.due_dt.year = new_rec->issue.issue_dt.year;
							}
							else
							{
								new_rec->issue.due_dt.month = 1;
								new_rec->issue.due_dt.year = new_rec->issue.issue_dt.year+1;
							}
						}
					}

					temp_stud = temp_stud->next;
				}
			}
			
			if(status==0)
			{
				printf("\nInvalid Member Number. Please try again\n");
			}
		}
	}
	else
	{
		printf("Enter HALL NUMBER: ");
		scanf("%d", &new_rec->loc.hall_num);

		printf("Enter RACK NUMBER: ");
		scanf("%d", &new_rec->loc.rack);

		printf("Enter ROW NUMBER: ");
		scanf("%d", &new_rec->loc.row);
	}

	new_rec->issue.renew_num = 0;
	return new_rec;
}

void add_bk(struct Bk_Record** bk_ptr, 
	struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr)
{
	struct Bk_Record* temp_bk = (*bk_ptr);
	struct Bk_Record* new_bk = (struct Bk_Record*)malloc(sizeof(struct Bk_Record));

	struct Mem_Record* temp_fac = (*fac_ptr);
	struct Mem_Record* temp_stud = (*stud_ptr);

	new_bk = bk_input(new_bk, &temp_fac, &temp_stud);

	int status;

	temp_bk = (*bk_ptr);
	status = 0;

	while(temp_bk->next!=NULL && status==0)
	{
		if(strcmp(new_bk->bk_subject, temp_bk->bk_subject)==0)
		{
			status = 1;
			new_bk->next = temp_bk->next;
			temp_bk->next = new_bk;
		}

		temp_bk = temp_bk->next;
	}

	if(status==0)
	{
		new_bk->next = temp_bk->next;
		temp_bk->next = new_bk;
	}
}

void add_mem(struct Mem_Record** fac_ptr, 
	struct Mem_Record** stud_ptr)
{
	struct Mem_Record* temp_fac = (*fac_ptr);
	struct Mem_Record* temp_stud = (*stud_ptr);
	struct Mem_Record* new_mem = (struct Mem_Record*)malloc(sizeof(struct Mem_Record));
	
	int mem_type;
	printf("Enter DESIGNATION (0: student, 1: faculty): ");
	scanf("%d", &mem_type);

	if(mem_type==0)
	{
		new_mem = mem_input(new_mem, mem_type);
			
		if(temp_stud==NULL)
		{
			(*stud_ptr) = temp_stud = new_mem;
		}
		else
		{
			temp_stud->next = new_mem;
			temp_stud = temp_stud->next;
		}
	}
	else
	{
		new_mem = mem_input(new_mem, mem_type);

		if(temp_fac==NULL)
		{
			(*fac_ptr) = temp_fac = new_mem;
		}
		else
		{
			temp_fac->next = new_mem;
			temp_fac = temp_fac->next;
		}
	}
}

void max_issue_mem(struct Mem_Record* fac_list, 
	struct Mem_Record* stud_list)
{
	struct Mem_Record* max_mem = NULL;

	int max_issue;

	max_issue = 0;

	if(fac_list!=NULL)
	{
		printf("\n\nMaximum Issues (Faculty)\n");
		while(fac_list!=NULL)
		{
			if(fac_list->issue_bk_num >= max_issue)
			{
				max_mem = fac_list;
				max_issue = fac_list->issue_bk_num;
			}

			fac_list = fac_list->next;
		}

		display_mem(max_mem, 0);
	}

	max_issue = 0;

	if(stud_list!=NULL)
	{
		printf("\n\nMaximum Issues (Student)\n");
		while(stud_list!=NULL)
		{
			if(stud_list->issue_bk_num >= max_issue)
			{
				max_mem = stud_list;
				max_issue = stud_list->issue_bk_num;
			}

			stud_list = stud_list->next;
		}

		display_mem(max_mem, 0);
	}
}

void display_mem(struct Mem_Record* mem_rec, int key)
{
	printf("Member Id: %d\n", mem_rec->member_id);
	printf("Member Name: %s\n", mem_rec->name);
	printf("Member Department: %s\n", mem_rec->dept);
	printf("Member Books Issued: %d\n", mem_rec->issue_bk_num);

	if(key==1)
	{	
		if(mem_rec->design==0)
		{
			printf("Member Role: Student\n");
		}
		else
		{
			printf("Member Role: Faculty\n");
		}
	}
}

void sort_display(struct Mem_Record* fac_list, struct Mem_Record* stud_list)
{
	int count_fac, count_stud; 
	count_fac = sort(&fac_list);
	count_stud = sort(&stud_list);

	if(count_fac>count_stud)
	{
		while(stud_list!=NULL)
		{
			if(fac_list->issue_bk_num >= stud_list->issue_bk_num)
			{
				display_mem(fac_list, 1);
				fac_list = fac_list->next;
			}
			else
			{
				display_mem(stud_list, 1);
				stud_list = stud_list->next;	
			}

			while(fac_list!=NULL)
			{
				display_mem(fac_list, 1);
				fac_list = fac_list->next;
			}
		}
	}
	else
	{
		while(fac_list!=NULL)
		{
			if(fac_list->issue_bk_num >= stud_list->issue_bk_num)
			{
				display_mem(fac_list, 1);
				fac_list = fac_list->next;
			}
			else
			{
				display_mem(stud_list, 1);
				stud_list = stud_list->next;	
			}

			while(stud_list!=NULL)
			{
				display_mem(stud_list, 1);
				stud_list = stud_list->next;
			}
		}
	}
}

int sort(struct Mem_Record** mem_list)
{
	int count, status, i, j;
	struct Mem_Record* temp_mem = NULL;
	struct Mem_Record* temp_swap = NULL;
	struct Mem_Record* prev_mem = NULL;
	
	count = 0;
	temp_mem = (*mem_list);
	
	while(temp_mem!=NULL)
	{
		count++;
		temp_mem = temp_mem->next;
	}

	for(i=0; i<count; i++)
	{
		prev_mem = temp_mem = (*mem_list);
		for(j=0; j<count-i-1; j++)
		{
			if(temp_mem->issue_bk_num < temp_mem->next->issue_bk_num)
			{
				if(j==0)
				{
					temp_swap = temp_mem->next;
					temp_mem->next = temp_mem->next->next;
					temp_swap->next = temp_mem; 

					prev_mem = temp_swap;
				}
				else
				{
					prev_mem->next = temp_mem->next;
					temp_swap = temp_mem->next;
					temp_mem->next = temp_mem->next->next;
					temp_swap->next = temp_mem; 

					prev_mem = prev_mem->next;
				}
			}
			else if(temp_mem->issue_bk_num==temp_mem->next->issue_bk_num)
			{
				if(strcmp(temp_mem->name, temp_mem->next->name)>0)
				{
					if(j==0)
					{
						temp_swap = temp_mem->next;
						temp_mem->next = temp_mem->next->next;
						temp_swap->next = temp_mem; 

						prev_mem = temp_swap;
					}
					else
					{
						prev_mem->next = temp_mem->next;
						temp_swap = temp_mem->next;
						temp_mem->next = temp_mem->next->next;
						temp_swap->next = temp_mem; 

						prev_mem = prev_mem->next;
					}
				}
			}
			else
			{
				prev_mem = prev_mem->next;
				temp_mem = temp_mem->next;
			}
		}
	}

	return count;
}

void chk_available(struct Bk_Record* bk_list)
{
	char bk_name[char_size];
	int status;

	getchar();
	printf("Enter the name of the book: ");
	gets(bk_name);

	status = 0;
	while(bk_list!=NULL && status==0)
	{
		if(bk_list->flag==1 && strcmp(bk_list->Name, bk_name)==0)
		{
			status = 1;
			printf("The book is available\n");
			printf("BOOK NAME: %s\n", bk_list->Name);
			printf("BOOK SUBJECT: %s\n", bk_list->bk_subject);
			printf("BOOK ACCESSION NUMBER %d\n", bk_list->acc_num);
			printf("PUBLSIH YEAR: %d\n", bk_list->publish_yr);
		}

		bk_list = bk_list->next;
	}

	if(status==0)
	{
		printf("The book is not available\n");
	}
}

void bk_issue(struct Bk_Record** bk_ptr, 
	struct Mem_Record** mem_ptr)
{
	int id, mem_status, bk_status, days_dif;
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char bk_name[char_size];
	
	printf("\nEnter Member ID: ");
	scanf("%d", &id);

	struct Bk_Record* bk_list = (*bk_ptr);
	struct Mem_Record* mem_list = (*mem_ptr);

	mem_status = 0;
	while(mem_list!=NULL && mem_status==0)
	{
		if(mem_list->member_id==id)
		{
			mem_status = 1;
			bk_status = 0;

			getchar();
			printf("Enter the name of the book: ");
			gets(bk_name);

			while(bk_list!=NULL && bk_status==0)
			{
				if(bk_list->flag==1 && strcmp(bk_list->Name, bk_name)==0)
				{
					bk_status = 1;
					bk_list->flag = 0;

					printf("The book is available in Hall: %d, Row: %d, Rack: %d\n", 
						bk_list->loc.hall_num, bk_list->loc.row, bk_list->loc.rack);
					
					printf("BOOK NAME: %s\n", bk_list->Name);
					printf("BOOK SUBJECT: %s\n", bk_list->bk_subject);
					printf("BOOK ACCESSION NUMBER %d\n", bk_list->acc_num);
					printf("PUBLSIH YEAR: %d\n", bk_list->publish_yr);

					bk_list->issue.member_id = id;
					bk_list->issue.issue_dt.day = cur_day;
					bk_list->issue.issue_dt.month = cur_month;
					bk_list->issue.issue_dt.year = cur_year;

					if(mem_list->design==0)
					{
						days_dif = bk_list->issue.issue_dt.day + 14 - month_days[bk_list->issue.issue_dt.month-1];
						if(days_dif<=0)
						{
							bk_list->issue.due_dt.day = bk_list->issue.issue_dt.day + 14;
							bk_list->issue.due_dt.month = bk_list->issue.issue_dt.month;
							bk_list->issue.due_dt.year = bk_list->issue.issue_dt.year;
						}
						else
						{
							bk_list->issue.due_dt.day = days_dif;
							if(bk_list->issue.issue_dt.month!=12)
							{
								bk_list->issue.due_dt.month = bk_list->issue.issue_dt.month+1;
								bk_list->issue.due_dt.year = bk_list->issue.issue_dt.year;
							}
							else
							{
								bk_list->issue.due_dt.month = 1;
								bk_list->issue.due_dt.year = bk_list->issue.issue_dt.year+1;
							}
						}
					}
					else
					{
						bk_list->issue.due_dt.day = bk_list->issue.issue_dt.day;
						if(bk_list->issue.issue_dt.month!=12)
						{
							bk_list->issue.due_dt.month = bk_list->issue.issue_dt.month+1;
							bk_list->issue.due_dt.year = bk_list->issue.issue_dt.year;
						}
						else
						{
							bk_list->issue.due_dt.month = 1;
							bk_list->issue.due_dt.year = bk_list->issue.issue_dt.year+1;
						}
					}

					printf("\nISSUE DATE: %d-%d-%d", bk_list->issue.issue_dt.day,
						bk_list->issue.issue_dt.month, bk_list->issue.issue_dt.year);

					printf("\nDUE DATE: %d-%d-%d\n", bk_list->issue.due_dt.day,
						bk_list->issue.due_dt.month, bk_list->issue.due_dt.year);

					mem_list->issue_bk_num++;
				}

				bk_list = bk_list->next;
			}

			if(bk_status==0)
			{
				printf("\nThe book is not available\n");
			}
		}

		mem_list = mem_list->next;
	}

	if(mem_status==0)
	{
		printf("\nThe member id is invalid\n");
	}
}

void bk_renew(struct Bk_Record** bk_ptr, 
	struct Mem_Record* mem_list)
{
	struct Bk_Record* temp_bk = (*bk_ptr);
	struct Mem_Record* temp_mem = NULL;

	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int days_dif, fine, mem_id, acc_num, bk_status, mem_status, tot_due_days, tot_cur_days, i;

	printf("\nEnter your MEMBER ID and the BOOK ACCESSION NUMBER: ");
	scanf("%d%d", &mem_id, &acc_num);

	bk_status = 0;
	while(temp_bk!=NULL && bk_status==0)
	{
		if(temp_bk->acc_num==acc_num)
		{
			bk_status = 1;
			
			if(temp_bk->flag==0 && temp_bk->issue.member_id==mem_id)
			{
				tot_due_days = temp_bk->issue.due_dt.day;
				tot_cur_days = cur_day;

				for(i=0; i<cur_month-1; i++)
				{
					tot_cur_days += month_days[i];
				}

				for(i=0; i<temp_bk->issue.due_dt.month - 1; i++)
				{
					tot_due_days += month_days[i]; 
				}

				if(tot_cur_days - tot_due_days>0)
				{
					if(temp_bk->issue.renew_num<3)
					{
						temp_bk->issue.issue_dt.day = cur_day;
						temp_bk->issue.issue_dt.month = cur_month;
						temp_bk->issue.issue_dt.year = cur_year;

						mem_status = 0;
						temp_mem = mem_list;
						
						while(temp_mem!=NULL && mem_status==0)
						{
							if(temp_mem->member_id==mem_id)
							{
								temp_mem->issue_bk_num++;
								mem_status = 1;

								if(temp_mem->design==1)
								{
									days_dif = temp_bk->issue.issue_dt.day + 14 - month_days[temp_bk->issue.issue_dt.month-1];
									if(days_dif<=0)
									{
										temp_bk->issue.due_dt.day = temp_bk->issue.issue_dt.day + 14;
										temp_bk->issue.due_dt.month = temp_bk->issue.issue_dt.month;
										temp_bk->issue.due_dt.year = temp_bk->issue.issue_dt.year;
									}
									else
									{
										temp_bk->issue.due_dt.day = days_dif;
										if(temp_bk->issue.issue_dt.month!=12)
										{
											temp_bk->issue.due_dt.month = temp_bk->issue.issue_dt.month+1;
											temp_bk->issue.due_dt.year = temp_bk->issue.issue_dt.year;
										}
										else
										{
											temp_bk->issue.due_dt.month = 1;
											temp_bk->issue.due_dt.year = temp_bk->issue.issue_dt.year+1;
										}
									}
								}
								else
								{
									temp_bk->issue.due_dt.day = temp_bk->issue.issue_dt.day;
									if(temp_bk->issue.issue_dt.month!=12)
									{
										temp_bk->issue.due_dt.month = temp_bk->issue.issue_dt.month+1;
										temp_bk->issue.due_dt.year = temp_bk->issue.issue_dt.year;
									}
									else
									{
										temp_bk->issue.due_dt.month = 1;
										temp_bk->issue.due_dt.year = temp_bk->issue.issue_dt.year+1;
									}
								}

								printf("\nBook Renewed\n");

								printf("\nNEW ISSUE DATE: %d-%d-%d", temp_bk->issue.issue_dt.day,
									temp_bk->issue.issue_dt.month, temp_bk->issue.issue_dt.year);

								printf("\nNEW DUE DATE: %d-%d-%d\n", temp_bk->issue.due_dt.day,
									temp_bk->issue.due_dt.month, temp_bk->issue.due_dt.year);
							}	

							temp_mem = mem_list->next;
						}
					}
					else
					{
						printf("\nYou have exceeded the maximum renews allowed\n");
					}
				}
				else
				{
					fine = (tot_due_days-tot_cur_days) * 0.5;
					printf("The book was due already. Please pay fine of %d\n", fine);
				}
			}
			else
			{
				printf("\nNo book issue with Accession Number: %d\n", acc_num);
			}
		}

		temp_bk = temp_bk->next;
	}
}