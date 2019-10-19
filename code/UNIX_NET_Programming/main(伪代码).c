void main()
{
  int n; 
  char title[50];
  int nlen_en，nlen_ch;
  char** ppData_en;
  char** ppData_ch;
  *ppData_en=NULL; *ppData_ch=NULL;
  nlen_en=0; nlen_ch=0;
  gets(title);
  //获取论文
  n=GetAPaper(title，&nlen_en，ppData_en);
  if(!(*ppData_en))
	exit(2);
  else if(n!=0)
	{ ShowErr(n); exit(1);}
  //翻译论文
  n=TransADoc(nlen_en，*ppData，&nlen_ch，ppData_ch);
  if(!(*ppData_ch))
	exit(3);
  else if(n!=0)
	{ ShowErr(n); exit(1);}
  //保存论文
  SavePaper(nlen_ch，*ppData_ch);
  if(*ppnData_en!=NULL)
		delete *ppnData_en;
  if(*ppnData_ch!=NULL)
		delete *ppnData_ch;
}
