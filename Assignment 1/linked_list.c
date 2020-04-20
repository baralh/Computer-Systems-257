#include<stdio.h>
#include<stdlib.h>
 

struct node {
    int data;
    struct node * prev;
    struct node * next;
}*head, *last;
 
 
 int count = 0;

void append(int num)
{
    struct node *temp,*right;
    temp= (struct node *)malloc(sizeof(struct node));
    temp->data=num;
    //temp->next=head;
   // head=temp;		
    
    right=(struct node *)head;
    while(right->next != NULL)
    right=right->next;
    right->next =temp;
    right=temp;
    right->next=NULL;
}

void append2(int num)

{

   struct node *temp,*right;
   temp=(struct node *)malloc(sizeof(struct node));
   temp->data=num;
   
   temp->next=head;						
   head=temp;	



} 
 
 
void add( int num )
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    if (head== NULL)
    {
    head=temp;
    head->next=NULL;
    }
    else
    {
    temp->next=head;
    head=temp;
    }
}
void addafter(int num, int loc)
{
    int i;
    struct node *temp,*left,*right;
    right=head;
    for(i=1;i<loc;i++)
    {
    left=right;
    right=right->next;
    }
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    left->next=temp;
    left=temp;
    left->next=right;
    return;
}
 
 
 
void insert(int num)
{
    int c=0;
    struct node *temp;
    temp=head;
    if(temp==NULL)
    {
    add(num);
    }
    else
    {
    while(temp!=NULL)
    {
        if(temp->data<num)
        c++;
        temp=temp->next;
    }
    if(c==0)
        add(num);

    else if(c<count())
        addafter(num,++c);
    else
        append(num);
    }
}
 
 
 
int delete(int num)
{
    struct node *temp, *prev;
    temp=head;
    while(temp!=NULL)
    {
    if(temp->data==num)
    {
        if(temp==head)
        {
        head=temp->next;
        free(temp);
        return 1;
        }
        else
        {
        prev->next=temp->next;
        free(temp);
        return 1;
        }
    }
    else
    {
        prev=temp;
        temp= temp->next;
    }
    }
    return 0;
}
 
int deletelast(struct node *head,int num){

	struct node *x = NULL;
	struct node *temp = head;

	while(temp)

	{	
	
	if(temp->next && temp->next->data == num)

	x=temp;
	temp=temp->next;

	}	

	if (x)
	{

	temp = x->next;
	x->next = x->next->next;
	free(temp);

	}	


	else {

	if (head &&(head->data)==num)
	{
		delete(num);
	}

	}





int  noDuplicate(int num)
{

	int c=0;
	int duplicate=0;
	struct node *temp;
	temp=head;
	if(temp==NULL)
	{
		add(num);

	}
	else
	{
	while(temp!=NULL&&duplicate==0)

	{
	if(temp->data<num)
		c++;

	if(temp->data==num)
		duplicate=1;
		temp=temp->next;

	}
	

	if(duplicate == 1)
	{

	}	

	else if(c==0)
		add(num);
	
	else
		insert(num);

}

}



 
void  display(struct node *r)
{
    r=head;
    if(r==NULL)
    {
    return;
    }
    while(r!=NULL)
    {
    printf("%d ",r->data);
    r=r->next;
    }
    printf("\n");
}
 
 
int count()
{
    struct node *n;
    int c=0;
    n=head;
    while(n!=NULL)
    {
    n=n->next;
    c++;
    }
    return c;
}
 
 
int  main()
{
    int i,num;
    struct node *n;
    head=NULL;
    int count = 0;

    while(1)
    {
    printf("\nList Operations\n");
    printf("===============\n");
    printf("1.Insert\n");
    printf("2.Display\n");
    printf("3.Size\n");
    printf("4.Delete\n");
    printf("5.Exit\n");
    printf("Enter your choice : ");
    if(scanf("%d",&i)<=0){
        printf("Enter only an Integer\n");
        exit(0);
    } else {
        switch(i)
        {



        case 1:      printf("Enter the number to insert : ");
               
					  scanf("%d",&num);
                
					 if (head==NULL){
					
				    insert(num);		
//add(num);

	}
	else{

        append(num);
                 break;
             }


        case 2:     if(head==NULL)
                {
                printf("List is Empty\n");
                }
                else
                {
                printf("Element(s) in the list are : ");
                }
                display(n);
                break;
        case 3:     printf("Size of the list is %d\n",count());
                break;
        case 4:     if(head==NULL)
                printf("List is Empty\n");
                else{
                printf("Enter the number to delete : ");
                scanf("%d",&num);
                deletelast(head,num);
                    printf("%d deleted successfully\n",num);
               }
                break;
        case 5:     return 0;
        default:    printf("Invalid option\n");
        }
    }
    }
    return 0;
