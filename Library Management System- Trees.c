#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define char_size 50
#define tree_order 3
#define min_order 2

//today's date
#define cur_day 9
#define cur_month 4
#define cur_year 2021

typedef struct loc_struct
{
	int hall_num;
	int row ;
	int rack;
}bk_loc;

typedef struct date_struct
{
	int day;
	int month;
	int year;
}date;

typedef struct issue_struct
{
	int member_id;
	date issue_dt;
	date due_dt;
	int renew_num;
}issue_detail;

typedef struct Bk_Struct
{
	char name[char_size];
	char bk_subject[char_size];
	char bk_author[char_size][char_size]; //in case of multiple authors
	int acc_num;
	int publish_yr;
	int flag; //0:issued, 1:available
	bk_loc loc;
	issue_detail issue;
}Bk_Record;

typedef struct Mem_Struct
{
	char name[char_size];
	char dept[char_size];
	int design; //0: Student, 1: faculty
	int member_id;
	int issue_bk_num;
	int fine;
}Mem_Record;

typedef struct Bk_Tree_Node
{
	Bk_Record *books[tree_order];
	struct Bk_Tree_Node *child_ptr[tree_order+1];
	int size;
	struct Bk_Tree_Node *parent_ptr;
}Bk_Node;

typedef struct Stud_Tree_Node
{
	Mem_Record *student[tree_order];
	struct Stud_Tree_Node *child_ptr[tree_order+1];
	int size;
	struct Stud_Tree_Node *parent_ptr;
}Stud_Node;

typedef struct Fac_Tree_Node
{
	Mem_Record *faculty[tree_order];
	struct Fac_Tree_Node *child_ptr[tree_order+1];
	int size;
	struct Fac_Tree_Node *parent_ptr;
}Fac_Node;

void init_node(Bk_Node **bk_tree, Stud_Node **stud_tree, Fac_Node **fac_tree);
int menu(Bk_Node **bk_tree, Stud_Node **stud_tree, Fac_Node **fac_tree);
void init_db(Bk_Node **bktree_ptr, Stud_Node **studtree_ptr, Fac_Node **factree_ptr);
Bk_Node* create_bk_node();
Stud_Node* create_stud_node();
Fac_Node* create_fac_node();

void insert_bk(Bk_Node **bktree_ptr, Stud_Node **studtree_ptr, Fac_Node **factree_ptr);
void add_bk(Bk_Node** bktree_ptr, Bk_Record* temp);
Bk_Node* find_bk_leaf(Bk_Node *bk_tree, Bk_Record *key);
int is_bknode_full(Bk_Node *bk_temp);
Bk_Node* sort_bk_node(Bk_Node *bk_temp);
Bk_Node* update_bk_parent(Bk_Node *bk_temp, Bk_Record *key, Bk_Record *node_max);
void split_bk(Bk_Node** bk_tree, Bk_Node** bk_temp);
int search_bk(Bk_Node** bk_tree, Bk_Record* key);

void insert_mem(Stud_Node **studtree_ptr, Fac_Node **factree_ptr);
void display(Stud_Node* root);

void add_stud(Stud_Node** studtree_ptr, Mem_Record* temp);
Stud_Node* find_stud_leaf(Stud_Node *stud_tree, Mem_Record* key);
int is_studnode_full(Stud_Node *stud_temp);
Stud_Node* sort_stud_node(Stud_Node *stud_temp);
Stud_Node* update_stud_parent(Stud_Node *stud_temp, Mem_Record* key, Mem_Record* node_max);
void split_stud(Stud_Node** stud_tree, Stud_Node** stud_temp);
int search_stud(Stud_Node** stud_tree, Mem_Record* key, int mode);

void add_fac(Fac_Node** factree_ptr, Mem_Record* temp);
Fac_Node* find_fac_leaf(Fac_Node *fac_tree, Mem_Record* key);
int is_facnode_full(Fac_Node *fac_temp);
Fac_Node* sort_fac_node(Fac_Node *fac_temp);
Fac_Node* update_fac_parent(Fac_Node *fac_temp, Mem_Record* key, Mem_Record* node_max);
void split_fac(Fac_Node** fac_tree, Fac_Node** fac_temp);
int search_fac(Fac_Node** fac_tree, Mem_Record* key, int mode);

void max_issue_mem();

main()
{
    Bk_Node *bk_root = NULL;
	Stud_Node *stud_root = NULL;
	Fac_Node *fac_root = NULL;

	init_node(&bk_root, &stud_root, &fac_root);
	menu(&bk_root, &stud_root, &fac_root);
}

void init_node(Bk_Node **bk_tree, Stud_Node **stud_tree, Fac_Node **fac_tree)
{
	(*bk_tree) = create_bk_node();
	(*stud_tree) = create_stud_node();
	(*fac_tree) = create_fac_node();
}

Bk_Node* create_bk_node()
{
	int i;
	Bk_Node* new_bk = (Bk_Node*)malloc(sizeof(Bk_Node));
	new_bk->parent_ptr = NULL;
	for(i=0; i<tree_order; i++)
	{
		new_bk->child_ptr[i] = NULL;
		new_bk->books[i] = (Bk_Record*)malloc(sizeof(Bk_Record));
		strcpy(new_bk->books[i]->bk_subject, "\0");
		new_bk->books[i]->acc_num = 0;
	}
	new_bk->child_ptr[tree_order] = NULL;

	return new_bk;
}

Stud_Node* create_stud_node()
{
	int i;
	Stud_Node* new_stud = (Stud_Node*)malloc(sizeof(Stud_Node));
	new_stud->parent_ptr = NULL;
	for(i=0; i<tree_order; i++)
	{
		new_stud->child_ptr[i] = NULL;
		new_stud->student[i] = (Mem_Record*)malloc(sizeof(Mem_Record));
		strcpy(new_stud->student[i]->dept, "\0");
		new_stud->student[i]->member_id = 0;
	}
	new_stud->child_ptr[tree_order] = NULL;

	return new_stud;
}

Fac_Node* create_fac_node()
{
	int i;
	Fac_Node* new_fac = (Fac_Node*)malloc(sizeof(Fac_Node));
	new_fac->parent_ptr = NULL;
	for(i=0; i<tree_order; i++)
	{
		new_fac->child_ptr[i] = NULL;
		new_fac->faculty[i] = (Mem_Record*)malloc(sizeof(Mem_Record));
		strcpy(new_fac->faculty[i]->dept, "\0");
		new_fac->faculty[i]->member_id = 0;
	}
	new_fac->child_ptr[tree_order] = NULL;

	return new_fac;
}

menu(Bk_Node **bktree_ptr, Stud_Node **studtree_ptr, Fac_Node **factree_ptr)
{
	static int call_num = 0;
	Bk_Node *bk_tree = (*bktree_ptr);
	Stud_Node *stud_tree = (*studtree_ptr);
	Fac_Node *fac_tree = (*factree_ptr);

	if(call_num==0)
	{
		init_db(&bk_tree, &stud_tree, &fac_tree);
	}

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

	switch(x)
	{
		case 1: insert_bk(&bk_tree, &stud_tree, &fac_tree); //func to add a new book into the databse
				break;

		case 2: insert_mem(&stud_tree, &fac_tree); //func to add a new member into the database
				break;
/*
		case 3: max_issue_mem(); //will display maxmimum book borrowers (for faculty and student seperately)
				break;

		case 4: sort_display(); //will sort the display the members' list
				break;

		case 5: chk_available(); //will check for availibility of a book
				break;*/
	}

	menu(&bk_tree, &stud_tree, &fac_tree);
	return 0;
}

void init_db(Bk_Node **bktree_ptr, Stud_Node **studtree_ptr, Fac_Node **factree_ptr)
{
	Bk_Node *bk_tree = (*bktree_ptr);
	Stud_Node *stud_tree = (*studtree_ptr);
	Fac_Node *fac_tree = (*factree_ptr);

	char ch;

	printf("\nInitialsing Member Database\n\n");
	ch = 'y';
	while(ch=='y')
	{
		insert_mem(&stud_tree, &fac_tree);
		printf("Are there more entries?(y/n): ");
		scanf("%s", &ch);
	}

	display(stud_tree);

	getchar();

	printf("\nInitialsing Books Database\n\n");
	ch = 'y';
	while(ch=='y')
	{
		insert_bk(&bk_tree, &stud_tree, &fac_tree);
		printf("Are there more entries?(y/n): ");
		scanf("%s", &ch);
		getchar();
	}
}

void insert_bk(Bk_Node **bktree_ptr, Stud_Node **studtree_ptr, Fac_Node **factree_ptr)
{
	Bk_Record* temp = (Bk_Record*)malloc(sizeof(Bk_Record));
	char ath_ch = 'y';
	int search = 0, days_dif, author_num;
	int month_days[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	Mem_Record* key = (Mem_Record*)malloc(sizeof(Mem_Record));

	Bk_Node* bk_tree = (*bktree_ptr);
	Stud_Node* stud_tree = (*studtree_ptr);
	Fac_Node* fac_tree = (*factree_ptr);

	printf("Enter BOOK SUBJECT: ");
	gets(temp->bk_subject);

	printf("Enter BOOK NAME: ");
	gets(temp->name);

	author_num = 0;
	while(ath_ch=='y')
	{
		printf("Enter BOOK AUTHOR NAME: ");
	   	gets(temp->bk_author[author_num]);

	   	printf("Are there more authors?(y/n): ");
	   	scanf("%s", &ath_ch);

	   	author_num++;
	   	getchar();
	}

	printf("Enter BOOK ACCESSION NUMBER: ");
	scanf("%d", &temp->acc_num);

	printf("Enter BOOK PUBLISH YEAR: ");
	scanf("%d", &temp->publish_yr);

	printf("Enter BOOK STATUS (0: Issued, 1: Available): ");
	scanf("%d", &temp->flag);

	if(temp->flag==0)
	{
		while(search==0)
		{
			getchar();

			printf("Enter BORROWER DEPARTMENT: ");
			gets(key->dept);

			printf("Enter BORROWER MEMBER ID: ");
			scanf("%d", &temp->issue.member_id);

			key->member_id = temp->issue.member_id;

			search = search_stud(&stud_tree, key, 1);

			if(search==0)
			{
				search = search_fac(&fac_tree, key, 1);

				if(search==1)
				{
					printf("Enter ISSUE DATE dd{space}mm{space}yyyy: ");
					scanf("%d%d%d", &temp->issue.issue_dt.day,
						&temp->issue.issue_dt.month,
						&temp->issue.issue_dt.year);

					temp->issue.due_dt.day = temp->issue.issue_dt.day;
					if(temp->issue.issue_dt.month!=12)
					{
						temp->issue.due_dt.month = temp->issue.issue_dt.month+1;
						temp->issue.due_dt.year = temp->issue.issue_dt.year;
					}
					else
					{
						temp->issue.due_dt.month = 1;
						temp->issue.due_dt.year = temp->issue.issue_dt.year+1;
					}
				}
				else
				{
					printf("\nInvalid Member Number. Please try again\n");
				}
			}
			else
			{
				printf("Enter ISSUE DATE dd{space}mm{space}yyyy: ");
				scanf("%d%d%d", &temp->issue.issue_dt.day,
					&temp->issue.issue_dt.month,
					&temp->issue.issue_dt.year);

				days_dif = temp->issue.issue_dt.day + 14 - month_days[temp->issue.issue_dt.month-1];
				if(days_dif<=0)
				{
					temp->issue.due_dt.day = temp->issue.issue_dt.day + 14;
					temp->issue.due_dt.month = temp->issue.issue_dt.month;
					temp->issue.due_dt.year = temp->issue.issue_dt.year;
				}
				else
				{
					temp->issue.due_dt.day = days_dif;
					if(temp->issue.issue_dt.month!=12)
					{
						temp->issue.due_dt.month = temp->issue.issue_dt.month+1;
						temp->issue.due_dt.year = temp->issue.issue_dt.year;
					}
					else
					{
						temp->issue.due_dt.month = 1;
						temp->issue.due_dt.year = temp->issue.issue_dt.year+1;
					}
				}
			}
		}

		getchar();
	}
	else
	{
		printf("Enter HALL NUMBER: ");
		scanf("%d", &temp->loc.hall_num);

		printf("Enter RACK NUMBER: ");
		scanf("%d", &temp->loc.rack);

		printf("Enter ROW NUMBER: ");
		scanf("%d", &temp->loc.row);

		getchar();
	}

	add_bk(&bk_tree, temp);
}

void add_bk(Bk_Node** bktree_ptr, Bk_Record* temp)
{
	Bk_Node *bk_tree = (*bktree_ptr);
	Bk_Node *temp_bk = find_bk_leaf(bk_tree, temp);
	Bk_Record* max_node = NULL;
	int status, i;

	if(is_bknode_full(bk_tree)==0)
	{
		status = 0;
		for(i=0; i<tree_order && status==0; i++)
		{
			if(temp_bk->books[i]->bk_subject[0]=='\0')
			{
				temp_bk->books[i] = temp;
				status = 1;
			}

			if(status==0)
			{
				max_node = temp_bk->books[i];
			}
		}

		temp_bk = sort_bk_node(temp_bk);

        if(max_node!=NULL)
        {
            if((strcmp(temp->bk_subject, max_node->bk_subject)>0 ||
            (strcmp(temp->bk_subject, max_node->bk_subject)==0 &&
            temp->acc_num > max_node->acc_num)) && temp_bk->parent_ptr!=NULL)
            {
                temp_bk = update_bk_parent(temp_bk->parent_ptr, temp, max_node);
            }
        }
	}
	else
	{
		split_bk(&bk_tree, &temp_bk);
		add_bk(&temp_bk, temp);
	}
}

Bk_Node* find_bk_leaf(Bk_Node* bk_tree, Bk_Record* key)
{
	Bk_Node *ret_node;
	int i, status;

	if(bk_tree->child_ptr[0]==NULL)
	{
		ret_node = bk_tree;
	}
	else
	{
		status = 0;
		for(i=0; i<tree_order && status==0; i++)
		{
			if(strcmp(bk_tree->books[i]->bk_subject, key->bk_subject)>0 ||
				(strcmp(bk_tree->books[i]->bk_subject, key->bk_subject)==0 &&
				bk_tree->books[i]->acc_num > key->acc_num))
			{
				status = 1;
				ret_node = find_bk_leaf(bk_tree->child_ptr[i], key);
			}
			else if(bk_tree->books[i]->bk_subject[0]=='\0')
			{
				status = 1;
				ret_node = find_bk_leaf(bk_tree->child_ptr[i-1], key);
			}
		}

		if(status==0)
		{
			ret_node =  find_bk_leaf(bk_tree->child_ptr[tree_order], key);
		}
	}

	return  ret_node;
}

int is_bknode_full(Bk_Node* bk_tree)
{
	int retval = 1;
	if(strcmp(bk_tree->books[tree_order-1]->bk_subject, "\0")==0)
	{
		retval = 0;
	}

	return retval;
}

Bk_Node* sort_bk_node(Bk_Node *bk_temp)
{
	int i, j;
	Bk_Record* temp_data;
	Bk_Node* temp_child;

	for(i=0; i<tree_order && bk_temp->books[i]->bk_subject[0]!='\0'; i++)
	{
		for(j=i+1; j<tree_order && bk_temp->books[i]->bk_subject[0]!='\0'; j++)
		{
			if(strcmp(bk_temp->books[i]->bk_subject, bk_temp->books[j]->bk_subject)<0 ||
				(strcmp(bk_temp->books[i]->bk_subject, bk_temp->books[j]->bk_subject)==0 &&
				bk_temp->books[i]->bk_subject < bk_temp->books[j]->bk_subject))
			{
				temp_data = bk_temp->books[i];
				temp_child = bk_temp->child_ptr[i];

				bk_temp->books[i] = bk_temp->books[j];
				bk_temp->child_ptr[i] = bk_temp->child_ptr[j];

				bk_temp->books[j] = temp_data;
				bk_temp->child_ptr[j] = temp_child;
			}
		}
	}

	return bk_temp;
}

Bk_Node* update_bk_parent(Bk_Node *bk_temp, Bk_Record* key, Bk_Record* max_data)
{
	int i;
	Bk_Record* new_max;

	for(i=0; i<tree_order && bk_temp->books[i]->bk_subject[0]!='\0'; i++)
	{
		new_max = bk_temp->books[i];

		if(strcmp(bk_temp->books[i]->bk_subject, max_data->bk_subject)==0)
		{
			bk_temp->books[i] = key;
		}
	}

	if(strcmp(key->bk_subject, new_max->bk_subject)>0 && bk_temp->parent_ptr!=NULL)
	{
		bk_temp = update_bk_parent(bk_temp, key, max_data);
	}

	return bk_temp;
}

void split_bk(Bk_Node **bk_tree, Bk_Node **bk_temp)
{
	Bk_Node *node_ptr = NULL, *bk_root = (*bk_tree), *temp_ptr = (*bk_temp);

	if(bk_root->parent_ptr==NULL)
	{
		node_ptr = create_bk_node();
		bk_root = create_bk_node();

		bk_root->books[0] = temp_ptr->books[min_order-1];
		bk_root->child_ptr[0] = temp_ptr;
		bk_root->books[1] = temp_ptr->books[tree_order-1];
		bk_root->child_ptr[1] = node_ptr;

		temp_ptr->parent_ptr = bk_root;
		node_ptr->parent_ptr = temp_ptr->parent_ptr;
	}
	else
	{
		if(is_bknode_full(temp_ptr->parent_ptr)==1)
		{
			split_bk(&bk_root, &temp_ptr->parent_ptr);
		}
		else
		{
			node_ptr = create_bk_node();
			node_ptr->parent_ptr = temp_ptr->parent_ptr;

			int i, status;

			status = 0;
			for(i=0; i<tree_order && status==0; i++)
			{
				if(temp_ptr->parent_ptr->books[i]==temp_ptr->books[tree_order-1])
				{
					temp_ptr->parent_ptr->child_ptr[i] = node_ptr;
				}

				if(temp_ptr->books[i]->bk_subject[0]=='\0')
				{
					status = 1;

					temp_ptr->parent_ptr->books[i] = temp_ptr->books[min_order-1];
					temp_ptr->parent_ptr->child_ptr[i] = temp_ptr;
				}
			}
		}
	}

	node_ptr->books[0] = temp_ptr->books[min_order];
    strcpy(temp_ptr->books[min_order]->bk_subject, "\0");
    node_ptr->child_ptr[0] = temp_ptr->child_ptr[min_order];

    if(node_ptr->child_ptr[0]!=NULL)
    {
        node_ptr->child_ptr[0]->parent_ptr = node_ptr;
    }

    temp_ptr->child_ptr[min_order] = NULL;
	temp_ptr->parent_ptr = sort_bk_node(temp_ptr->parent_ptr);
}

int search_bk(Bk_Node** bk_tree, Bk_Record* key)
{
	Bk_Node *bk_root = (*bk_tree);
	Bk_Node *temp_bk = find_bk_leaf(bk_root, key);

	int i, status = 0;
	for(i=0; i<tree_order && status==0; i++)
	{
		if(temp_bk->books[i]->acc_num==key->acc_num)
		{
			status = 1;
		}
	}

	return status;
}

void insert_mem(Stud_Node **studtree_ptr, Fac_Node **factree_ptr)
{
	Mem_Record* temp = (Mem_Record*)malloc(sizeof(Mem_Record));

	printf("Entering MEMBER Details\n");

	printf("Enter DESIGNATION (0: student, 1: faculty): ");
	scanf("%d", &temp->design);

	getchar();
	printf("Enter Member NAME: ");
	gets(temp->name);

	printf("Enter DEPARTMENT: ");
	gets(temp->dept);

	printf("Enter MEMBER ID: ");
	scanf("%d", &temp->member_id);

	temp->issue_bk_num = 0;

	if(temp->design==0)
	{
		Stud_Node *stud_tree = (*studtree_ptr);
		add_stud(&stud_tree, temp);
	}
	else
	{
		Fac_Node *fac_tree = (*factree_ptr);
		add_fac(&fac_tree, temp);
	}
}

void add_stud(Stud_Node** studtree_ptr, Mem_Record* temp)
{
	Stud_Node *stud_tree = (*studtree_ptr);
	Stud_Node *temp_stud = find_stud_leaf(stud_tree, temp);
	Mem_Record* max_node = NULL;
	int status, i;
	printf("%d", is_studnode_full(stud_tree))
	if(is_studnode_full(stud_tree)==0)
	{
		status = 0;
		for(i=0; i<tree_order && status==0; i++)
		{
			if(temp_stud->student[i]->dept[0]=='\0')
			{
				temp_stud->student[i] = temp;
				status = 1;
			}

			if(status==0)
			{
				max_node = temp_stud->student[i];
			}
		}

		temp_stud = sort_stud_node(temp_stud);

        if(max_node!=NULL)
        {
            if((strcmp(temp->dept, max_node->dept)>0 ||
            (strcmp(temp->dept, max_node->dept)==0 &&
            temp->member_id > max_node->member_id)) && temp_stud->parent_ptr!=NULL)
            {
                temp_stud = update_stud_parent(temp_stud->parent_ptr, temp, max_node);
            }
        }
	}
	else
	{
		split_stud(&stud_tree, &temp_stud);
		add_stud(&temp_stud, temp);
	}
}

Stud_Node* find_stud_leaf(Stud_Node *stud_tree, Mem_Record *key)
{
	Stud_Node *ret_node;
	int i, status;
	printf("!");
	if(stud_tree->child_ptr[0]==NULL)
	{
		ret_node = stud_tree;
	}
	else
	{
		status = 0;
		for(i=0; i<tree_order-1 && status==0; i++)
		{
			if(strcmp(stud_tree->student[i]->dept, key->dept)>0 ||
				(strcmp(stud_tree->student[i]->dept, key->dept)==0 &&
				stud_tree->student[i]->member_id>key->member_id))
			{
				status = 1;
				ret_node = find_stud_leaf(stud_tree->child_ptr[i], key);
			}
			else if(stud_tree->student[i]->dept[0]=='\0')
			{
				status = 1;
				ret_node = find_stud_leaf(stud_tree->child_ptr[i-1], key);
			}
		}

		if(status==0)
		{
			ret_node =  find_stud_leaf(stud_tree->child_ptr[tree_order], key);
		}
	}

	return  ret_node;
}

int is_studnode_full(Stud_Node *stud_temp)
{
	int retval = 1;

	if(strcmp(stud_temp->student[tree_order-1]->dept, "\0")==0)
	{
		retval = 0;
	}

	return retval;
}

Stud_Node* sort_stud_node(Stud_Node *stud_temp)
{
	int i, j;
	Mem_Record* temp_data = NULL;
	Stud_Node* temp_child;

	for(i=0; i<tree_order && stud_temp->student[i]->dept[0]!='\0'; i++)
	{
		for(j=i+1; j<tree_order && stud_temp->student[j]->dept[0]!='\0'; j++)
		{
			if(strcmp(stud_temp->student[i]->dept, stud_temp->student[j]->dept)<0)
			{
				temp_data = stud_temp->student[i];
				temp_child = stud_temp->child_ptr[i];

				stud_temp->student[i] = stud_temp->student[j];
				stud_temp->child_ptr[i] = stud_temp->child_ptr[j];

				stud_temp->student[j] = temp_data;
				stud_temp->child_ptr[j] = temp_child;
			}
		}
	}

	return stud_temp;
}

Stud_Node* update_stud_parent(Stud_Node *stud_temp, Mem_Record *key, Mem_Record *node_max)
{
	int i;
	Mem_Record *new_max = NULL;

	for(i=0; i<tree_order && stud_temp->student[i]->dept[0]!='\0'; i++)
	{
		new_max = stud_temp->student[i];

		if(strcmp(stud_temp->student[i]->dept, node_max->dept)==0 &&
			stud_temp->student[i]->member_id==node_max->member_id)
		{
			stud_temp->student[i] = key;
		}
	}

	if((strcmp(key->dept, new_max->dept)>0 || (strcmp(key->dept, new_max->dept)==0 && key->member_id>new_max->member_id))
		&& stud_temp->parent_ptr!=NULL)
	{
		stud_temp = update_stud_parent(stud_temp, key, node_max);
	}

	return stud_temp;
}

void split_stud(Stud_Node **stud_tree, Stud_Node **stud_temp)
{
	Stud_Node *node_ptr = NULL, *stud_root = (*stud_tree), *temp_ptr = (*stud_temp);

	if(stud_root->parent_ptr==NULL)
	{
		node_ptr = create_stud_node();
		stud_root = create_stud_node();

		stud_root->student[0] = temp_ptr->student[min_order-1];
		stud_root->child_ptr[0] = temp_ptr;
		stud_root->student[1] = temp_ptr->student[tree_order-1];
		stud_root->child_ptr[1] = node_ptr;

		temp_ptr->parent_ptr = stud_root;
		node_ptr->parent_ptr = temp_ptr->parent_ptr;
	}
	else
	{
		if(is_studnode_full(temp_ptr->parent_ptr)==1)
		{
			split_stud(&stud_root, &temp_ptr->parent_ptr);
		}
		else
		{
			node_ptr = create_stud_node();
			node_ptr->parent_ptr = temp_ptr->parent_ptr;

			int i, status;

			status = 0;
			for(i=0; i<tree_order  && status==0; i++)
			{
				if(temp_ptr->parent_ptr->student[i]==temp_ptr->student[tree_order-1])
				{
					temp_ptr->parent_ptr->child_ptr[i] = node_ptr;
				}

				if(temp_ptr->student[i]->dept[0]=='\0')
				{
					status = 1;

					temp_ptr->parent_ptr->student[i] = temp_ptr->student[min_order-1];
					temp_ptr->parent_ptr->child_ptr[i] = temp_ptr;
				}
			}
		}
	}

	node_ptr->student[0] = temp_ptr->student[min_order];
    strcpy(temp_ptr->student[min_order]->dept, "\0");
    node_ptr->child_ptr[0] = temp_ptr->child_ptr[min_order];

    if(node_ptr->child_ptr[0]!=NULL)
    {
        node_ptr->child_ptr[0]->parent_ptr = node_ptr;
    }

    temp_ptr->child_ptr[min_order] = NULL;
	temp_ptr->parent_ptr = sort_stud_node(temp_ptr->parent_ptr);
}

int search_stud(Mem_Record* key, int mode)
{

	Stud_Node *temp_stud = find_stud_leaf(stud_root, key);

	int i, status = 0;
	for(i=0; i<tree_order-1 && status==0; i++)
	{
		if(temp_stud->student[i]->member_id==key->member_id)
		{
			status = 1;

			if(mode==1)
			{
				temp_stud->student[i]->issue_bk_num++;
			}
		}
	}

	return status;
}

void add_fac(Fac_Node** factree_ptr, Mem_Record* temp)
{
	Fac_Node *fac_tree = (*factree_ptr);
	Fac_Node *temp_fac = find_fac_leaf(fac_tree, temp);
	Mem_Record* max_node = NULL;
	int status, i;

	if(is_facnode_full(fac_tree)==0)
	{
		status = 0;
		for(i=0; i<tree_order && status==0; i++)
		{
			if(temp_fac->faculty[i]->dept[0]=='\0')
			{
				temp_fac->faculty[i] = temp;
				status = 1;
			}

			if(status==0)
			{
				max_node = temp_fac->faculty[i];
			}
		}

		temp_fac = sort_fac_node(temp_fac);

        if(max_node!=NULL)
        {
            if((strcmp(temp->dept, max_node->dept)>0 ||
            (strcmp(temp->dept, max_node->dept)==0 &&
            temp->member_id > max_node->member_id)) && temp_fac->parent_ptr!=NULL)
            {
                temp_fac = update_fac_parent(temp_fac->parent_ptr, temp, max_node);
            }
        }
	}
	else
	{
		split_fac(&fac_tree, &temp_fac);
		add_fac(&temp_fac, temp);
	}
}

Fac_Node* find_fac_leaf(Fac_Node *fac_tree, Mem_Record* key)
{
	Fac_Node *ret_node;
	int i, status;

	if(fac_tree->child_ptr[0]==NULL)
	{
		ret_node = fac_tree;
	}
	else
	{
		status = 0;
		for(i=0; i<tree_order && status==0; i++)
		{
			if(strcmp(fac_tree->faculty[i]->dept, key->dept)>0 ||
			(strcmp(fac_tree->faculty[i]->dept, key->dept)==0 &&
			fac_tree->faculty[i]->member_id > key->member_id))
			{
				status = 1;
				ret_node = find_fac_leaf(fac_tree->child_ptr[i], key);
			}
			else if(fac_tree->faculty[i]->dept[0]=='\0')
			{
				status = 1;
				ret_node = find_fac_leaf(fac_tree->child_ptr[i-1], key);
			}
		}

		if(status==0)
		{
			ret_node =  find_fac_leaf(fac_tree->child_ptr[tree_order], key);
		}
	}

	return  ret_node;
}


int is_facnode_full(Fac_Node *fac_temp)
{
	int retval = 1;

	if(strcmp(fac_temp->faculty[tree_order-1]->dept, "\0")==0)
	{
		retval = 0;
	}

	return retval;
}

Fac_Node* sort_fac_node(Fac_Node *fac_temp)
{
	int i, j;
	Mem_Record* temp_data = NULL;
	Fac_Node* temp_child;

	for(i=0; i<tree_order && fac_temp->faculty[i]->dept[0]!='\0'; i++)
	{
		for(j=i+1; j<tree_order && fac_temp->faculty[i]->dept[0]!='\0'; j++)
		{
			if(strcmp(fac_temp->faculty[i]->dept, fac_temp->faculty[j]->dept)<0)
			{
				temp_data = fac_temp->faculty[i];
				temp_child = fac_temp->child_ptr[i];

				fac_temp->faculty[i] = fac_temp->faculty[j];
				fac_temp->child_ptr[i] = fac_temp->child_ptr[j];

				fac_temp->faculty[j] = temp_data;
				fac_temp->child_ptr[j] = temp_child;
			}
		}
	}

	return fac_temp;
}

Fac_Node* update_fac_parent(Fac_Node *fac_temp, Mem_Record *key, Mem_Record *node_max)
{
	int i;
	Mem_Record* new_max;

	for(i=0; i<tree_order && fac_temp->faculty[i]->dept[0]!='\0'; i++)
	{
		new_max = fac_temp->faculty[i];

		if(strcmp(fac_temp->faculty[i]->dept, node_max->dept)==0)
		{
			fac_temp->faculty[i] = key;
		}
	}

	if((strcmp(key->dept, new_max->dept)>0 || (strcmp(key->dept, new_max->dept)==0
		&& key->member_id>new_max->member_id)) && fac_temp->parent_ptr!=NULL)
	{
		fac_temp = update_fac_parent(fac_temp, key, node_max);
	}

	return fac_temp;
}

void split_fac(Fac_Node **fac_tree, Fac_Node **fac_temp)
{
	Fac_Node *node_ptr = NULL, *fac_root = (*fac_tree), *temp_ptr = (*fac_temp);

	if(fac_root->parent_ptr==NULL)
	{
		node_ptr = create_fac_node();
		fac_root = create_fac_node();

		fac_root->faculty[0] = temp_ptr->faculty[min_order-1];
		fac_root->child_ptr[0] = temp_ptr;
		fac_root->faculty[1] = temp_ptr->faculty[tree_order-1];
		fac_root->child_ptr[1] = node_ptr;

		temp_ptr->parent_ptr = fac_root;
		node_ptr->parent_ptr = temp_ptr->parent_ptr;
	}
	else
	{
		if(is_facnode_full(temp_ptr->parent_ptr)==1)
		{
			split_fac(&fac_root, &temp_ptr->parent_ptr);
		}
		else
		{
			node_ptr = create_fac_node();
			node_ptr->parent_ptr = temp_ptr->parent_ptr;

			int i, status;

			status = 0;
			for(i=0; i<tree_order && status==0; i++)
			{
				if(temp_ptr->parent_ptr->faculty[i]==temp_ptr->faculty[tree_order-1])
				{
					temp_ptr->parent_ptr->child_ptr[i] = node_ptr;
				}

				if(temp_ptr->faculty[i]->dept[0]=='\0')
				{
					status = 1;

					temp_ptr->parent_ptr->faculty[i] = temp_ptr->faculty[min_order-1];
					temp_ptr->parent_ptr->child_ptr[i] = temp_ptr;
				}
			}
		}
	}

	node_ptr->faculty[0] = temp_ptr->faculty[min_order];
    strcpy(temp_ptr->faculty[min_order]->dept, "\0");
    node_ptr->child_ptr[0] = temp_ptr->child_ptr[min_order];

    if(node_ptr->child_ptr[0]!=NULL)
    {
        node_ptr->child_ptr[0]->parent_ptr = node_ptr;
    }

    temp_ptr->child_ptr[min_order] = NULL;
	temp_ptr->parent_ptr = sort_fac_node(temp_ptr->parent_ptr);
}

int search_fac(Fac_Node** fac_tree, Mem_Record* key, int mode)
{
	Fac_Node *fac_root = (*fac_tree);
	Fac_Node *temp_fac = find_fac_leaf(fac_root, key);

	int i, status = 0;

	for(i=0; i<tree_order-1 && status==0; i++)
	{
		printf("!");
		if(temp_fac->faculty[i]->member_id==key->member_id)
		{
			printf("1");
			status = 1;

			if(mode==1)
			{
				temp_fac->faculty[i]->issue_bk_num++;
			}
		}
	}

	return status;
}

void display(Stud_Node* root)
{
	Stud_Node* queue[50];
	int i;
	for(i=0; i<50; i++)
	{
		queue[i] = (Stud_Node*)malloc(sizeof(Stud_Node));
	}

	int f=0, r=-1, lvl = -1;

    r++;
	queue[r] = NULL;
	r++;
	queue[r] = stud_root;

	while(f<r)
	{
		if(queue[f]==NULL)
		{
		    lvl++;
			printf("\n\nLevel --> %d\n", lvl);
			if(queue[r]!=NULL)
			{
			    r++;
				queue[r]=NULL;
			}
			f++;
		}
		else
		{
			for(i=0; i<tree_order; i++)
			{
				if(strcpy(queue[f]->student[f], "\0"))
				{
					printf("%s-%d  ", queue[f]->student[i]->dept, queue[f]->student[i]->member_id);
				}

				if(queue[f]->child_ptr[i]!=NULL)
				{
					queue[++r]=queue[f]->child_ptr[i];
				}
			}
			printf("       ");
			f++;
		}
	}
}
