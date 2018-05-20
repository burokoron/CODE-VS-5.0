#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;

struct GAMEINFO{
	int cost[8];
	int ninryoku;
	char map[17][14];
	int ninja[2][2];
	int ninken[100][3];
	int soul[100][2];
}gameinfo_my, gameinfo_enemy;

struct SEARCHINFO{
	int dog_map[17][14];
	int dog_list[100][3];
	int soul_map[17][14];
	char rock_map[17][14];
	int bonus;
};

char move_pattern[]="URDLN";
int bunsin_pattern[]={1, 1, 15, 1, 1, 12, 15, 12};
int bunsin[2];
static int hit;

bool input(){
	int i, j, n, m;

	if (!(cin >> i)) return false;
	cin >> n;
	for(i=0;i<n;i++){
		cin >> gameinfo_my.cost[i];
	}
	cin >> gameinfo_my.ninryoku;
	cin >> n;
	cin >> m;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			cin >> gameinfo_my.map[i][j];
		}
	}
	cin >> n;
	for(i=0;i<n;i++){
		cin >> j;
		cin >> gameinfo_my.ninja[i][0];
		cin >> gameinfo_my.ninja[i][1];
	}
	cin >> n;
	for(i=0;i<n;i++){
		cin >> gameinfo_my.ninken[i][0];
		cin >> gameinfo_my.ninken[i][1];
		cin >> gameinfo_my.ninken[i][2];
	}
	cin >> n;
	for(i=0;i<n;i++){
		cin >> gameinfo_my.soul[i][0];
		cin >> gameinfo_my.soul[i][1];
	}
	for(i=0;i<8;i++){
		cin >> j;
	}

	cin >> gameinfo_enemy.ninryoku;
	cin >> n;
	cin >> m;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			cin >> gameinfo_enemy.map[i][j];
		}
	}
	cin >> n;
	for(i=0;i<n;i++){
		cin >> j;
		cin >> gameinfo_enemy.ninja[i][0];
		cin >> gameinfo_enemy.ninja[i][1];
	}
	cin >> n;
	for(i=0;i<n;i++){
		cin >> gameinfo_enemy.ninken[i][0];
		cin >> gameinfo_enemy.ninken[i][1];
		cin >> gameinfo_enemy.ninken[i][2];
	}
	cin >> n;
	for(i=0;i<n;i++){
		cin >> gameinfo_enemy.soul[i][0];
		cin >> gameinfo_enemy.soul[i][1];
	}
	for(i=0;i<8;i++){
		cin >> j;
	}


	return true;
}

void Init_searchinfo(struct SEARCHINFO *searchinfo, int flag){
	int i, j;

	if(flag==0){
		for(i=0;i<17;i++){
			for(j=0;j<14;j++){
				searchinfo->dog_map[i][j]=0;
			}
		}
		for(i=0;i<100;i++){
			if(gameinfo_my.ninken[i][1]!=0) searchinfo->dog_map[gameinfo_my.ninken[i][1]][gameinfo_my.ninken[i][2]]=1;
			searchinfo->dog_list[i][0]=gameinfo_my.ninken[i][0];
			searchinfo->dog_list[i][1]=gameinfo_my.ninken[i][1];
			searchinfo->dog_list[i][2]=gameinfo_my.ninken[i][2];
		}

		for(i=0;i<17;i++){
			for(j=0;j<14;j++){
				searchinfo->soul_map[i][j]=0;
			}
		}

		i=0;
		while(gameinfo_my.soul[i++][0]!=0) searchinfo->soul_map[gameinfo_my.soul[i-1][0]][gameinfo_my.soul[i-1][1]]=1;

		for(i=0;i<17;i++){
			for(j=0;j<14;j++){
				searchinfo->rock_map[i][j]=gameinfo_my.map[i][j];
			}
		}
	}
	else{
		for(i=0;i<17;i++){
			for(j=0;j<14;j++){
				searchinfo->dog_map[i][j]=0;
			}
		}

		for(i=0;i<100;i++){
			searchinfo->dog_map[gameinfo_enemy.ninken[i][1]][gameinfo_enemy.ninken[i][2]]=1;
			searchinfo->dog_list[i][0]=gameinfo_enemy.ninken[i][0];
			searchinfo->dog_list[i][1]=gameinfo_enemy.ninken[i][1];
			searchinfo->dog_list[i][2]=gameinfo_enemy.ninken[i][2];
		}

		for(i=0;i<17;i++){
			for(j=0;j<14;j++){
				searchinfo->soul_map[i][j]=0;
			}
		}

		i=0;
		while(gameinfo_enemy.soul[i++][0]!=0) searchinfo->soul_map[gameinfo_enemy.soul[i-1][0]][gameinfo_enemy.soul[i-1][1]]=1;

		for(i=0;i<17;i++){
			for(j=0;j<14;j++){
				searchinfo->rock_map[i][j]=gameinfo_enemy.map[i][j];
			}
		}
	}

	searchinfo->bonus=0;
}

int Soul_value_single(struct SEARCHINFO *searchinfo, int *ninja){
	int k=100, flag=1, soul_search_map[17][14]={0};

	for(int i=1;i<16;i++){
		for(int j=1;j<13;j++){
			if(searchinfo->soul_map[i][j]==1) soul_search_map[i][j]=100;
		}
	}

	while(flag!=0){
		flag=0;
		for(int i=1;i<16;i++){
			for(int j=0;j<13;j++){
				if(soul_search_map[i][j]==k){
					if(soul_search_map[i-1][j]==0){
						soul_search_map[i-1][j]=k-10;
						flag++;
					}
					if(soul_search_map[i+1][j]==0){
						soul_search_map[i+1][j]=k-10;
						flag++;
					}
					if(soul_search_map[i][j-1]==0){
						soul_search_map[i][j-1]=k-10;
						flag++;
					}
					if(soul_search_map[i][j+1]==0){
						soul_search_map[i][j+1]=k-10;
						flag++;
					}
				}
			}
		}
		k-=10;
	}

	return soul_search_map[ninja[0]][ninja[1]];
}

void Dog_move_single(struct SEARCHINFO *searchinfo, int *ninja){
	int k=1, flag=1, dog_num=0, move_dog_num=0, dog_move_map[17][14]={0};

	dog_move_map[ninja[0]][ninja[1]]=1;

	while(flag!=0){
		flag=0;
		for(int i=1;i<16;i++){
			for(int j=1;j<13;j++){
				if(dog_move_map[i][j]==k){
					if(searchinfo->rock_map[i-1][j]=='_'&&dog_move_map[i-1][j]==0){
						dog_move_map[i-1][j]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i+1][j]=='_'&&dog_move_map[i+1][j]==0){
						dog_move_map[i+1][j]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i][j-1]=='_'&&dog_move_map[i][j-1]==0){
						dog_move_map[i][j-1]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i][j+1]=='_'&&dog_move_map[i][j+1]==0){
						dog_move_map[i][j+1]=k+1;
						flag++;
					}
				}
			}
		}
		k++;
	}

	int i=0;
	while(searchinfo->dog_list[i++][1]!=0) dog_num++;

	k=1;
	while(move_dog_num<dog_num){
		int i=0;
		while(searchinfo->dog_list[i++][1]!=0){
			if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]==k&&k>1){
				if(dog_move_map[searchinfo->dog_list[i-1][1]-1][searchinfo->dog_list[i-1][2]]==k-1
					&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]-1][searchinfo->dog_list[i-1][2]]==0){
						searchinfo->dog_map[searchinfo->dog_list[i-1][1]-1][searchinfo->dog_list[i-1][2]]=1;
						searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
						searchinfo->dog_list[i-1][1]--;
				}else if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]-1]==k-1
						&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]-1]==0){
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]-1]=1;
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
							searchinfo->dog_list[i-1][2]--;
				}else if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]+1]==k-1
						&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]+1]==0){
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]+1]=1;
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
							searchinfo->dog_list[i-1][2]++;
				}else if(dog_move_map[searchinfo->dog_list[i-1][1]+1][searchinfo->dog_list[i-1][2]]==k-1
						&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]+1][searchinfo->dog_list[i-1][2]]==0){
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]+1][searchinfo->dog_list[i-1][2]]=1;
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
							searchinfo->dog_list[i-1][1]++;
				}
				move_dog_num++;
			}
			else if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]==1&&k==1) move_dog_num++;
		}
		k++;if(k>100) break;
	}
}

void Copy_searchinfo(struct SEARCHINFO *_searchinfo, struct SEARCHINFO *searchinfo){
	for(int i=0;i<17;i++){
		for(int j=0;j<14;j++){
			_searchinfo->dog_map[i][j]=searchinfo->dog_map[i][j];
			_searchinfo->soul_map[i][j]=searchinfo->soul_map[i][j];
			_searchinfo->rock_map[i][j]=searchinfo->rock_map[i][j];
		}
	}

	for(int i=0;i<100;i++){
		_searchinfo->dog_list[i][0]=searchinfo->dog_list[i][0];
		_searchinfo->dog_list[i][1]=searchinfo->dog_list[i][1];
		_searchinfo->dog_list[i][2]=searchinfo->dog_list[i][2];
	}

	_searchinfo->bonus=searchinfo->bonus;
}

int Search_single(struct SEARCHINFO *searchinfo, int *ninja, char moves[20][20], int deep, int depth){
	int best=-10000, value;
	struct SEARCHINFO _searchinfo;

	Copy_searchinfo(&_searchinfo, searchinfo);

	if(deep!=0){
		if(_searchinfo.rock_map[ninja[0]][ninja[1]]=='W') return -9999+deep;
		if(_searchinfo.rock_map[ninja[0]][ninja[1]]=='O'){
			int flag=0;
			if(flag==0&&moves[19][deep-1]=='U'
				&&_searchinfo.rock_map[ninja[0]-1][ninja[1]]=='_'
				&&_searchinfo.dog_map[ninja[0]-1][ninja[1]]==0){
					_searchinfo.rock_map[ninja[0]][ninja[1]]='_';
					_searchinfo.rock_map[ninja[0]-1][ninja[1]]='O';
					flag=1;
			}
			if(flag==0&&moves[19][deep-1]=='D'
				&&_searchinfo.rock_map[ninja[0]+1][ninja[1]]=='_'
				&&_searchinfo.dog_map[ninja[0]+1][ninja[1]]==0){
					_searchinfo.rock_map[ninja[0]][ninja[1]]='_';
					_searchinfo.rock_map[ninja[0]+1][ninja[1]]='O';
					flag=1;
			}
			if(flag==0&&moves[19][deep-1]=='L'
				&&_searchinfo.rock_map[ninja[0]][ninja[1]-1]=='_'
				&&_searchinfo.dog_map[ninja[0]][ninja[1]-1]==0){
					_searchinfo.rock_map[ninja[0]][ninja[1]]='_';
					_searchinfo.rock_map[ninja[0]][ninja[1]-1]='O';
					flag=1;
			}
			if(flag==0&&moves[19][deep-1]=='R'
				&&_searchinfo.rock_map[ninja[0]][ninja[1]+1]=='_'
				&&_searchinfo.dog_map[ninja[0]][ninja[1]+1]==0){
					_searchinfo.rock_map[ninja[0]][ninja[1]]='_';
					_searchinfo.rock_map[ninja[0]][ninja[1]+1]='O';
					flag=1;
			}
			if(flag==0) return -9999+deep;
		}

		if(_searchinfo.soul_map[ninja[0]][ninja[1]]==1){
			_searchinfo.bonus+=(10-deep)*100;
			_searchinfo.soul_map[ninja[0]][ninja[1]]=0;
		}
	}

	if(((deep-hit)%2==0&&deep>=hit)||deep==hit){
		Dog_move_single(&_searchinfo, ninja);
		if(_searchinfo.dog_map[ninja[0]][ninja[1]]!=0) return -9999+deep;
	}

	if(depth<=deep) return Soul_value_single(&_searchinfo, ninja)+_searchinfo.bonus;

	moves[19][deep+1]='\0';
	for(int i=0;i<5;i++){
		if(deep>=hit&&i==4) break;
		moves[19][deep]=move_pattern[i];
		switch(i){
			case 0:
				ninja[0]--;
				value=Search_single(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[0]++;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 1:
				ninja[1]++;
				value=Search_single(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[1]--;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 2:
				ninja[0]++;
				value=Search_single(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[0]--;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 3:
				ninja[1]--;
				value=Search_single(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[1]++;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 4:
				value=Search_single(&_searchinfo, ninja, moves, deep+1, depth);
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
		}
	}

	return best;
}

int Soul_value_double(struct SEARCHINFO *searchinfo, int ninja[2][2]){
	int k=100, flag=1, soul_search_map[17][14]={0};

	for(int i=1;i<16;i++){
		for(int j=1;j<13;j++){
			if(searchinfo->soul_map[i][j]==1) soul_search_map[i][j]=100;
		}
	}

	while(flag!=0){
		flag=0;
		for(int i=1;i<16;i++){
			for(int j=0;j<13;j++){
				if(soul_search_map[i][j]==k){
					if(soul_search_map[i-1][j]==0){
						soul_search_map[i-1][j]=k-10;
						flag++;
					}
					if(soul_search_map[i+1][j]==0){
						soul_search_map[i+1][j]=k-10;
						flag++;
					}
					if(soul_search_map[i][j-1]==0){
						soul_search_map[i][j-1]=k-10;
						flag++;
					}
					if(soul_search_map[i][j+1]==0){
						soul_search_map[i][j+1]=k-10;
						flag++;
					}
				}
			}
		}
		k-=10;
	}

	return soul_search_map[ninja[0][0]][ninja[0][1]]+soul_search_map[ninja[1][0]][ninja[1][1]];
}

void Dog_move_double(struct SEARCHINFO *searchinfo, int ninja[2][2]){
	int k=1, flag=1, dog_num=0, move_dog_num=0, dog_move_map[17][14]={0};

	dog_move_map[ninja[0][0]][ninja[0][1]]=1;
	dog_move_map[ninja[1][0]][ninja[1][1]]=1;

	while(flag!=0){
		flag=0;
		for(int i=1;i<16;i++){
			for(int j=1;j<13;j++){
				if(dog_move_map[i][j]==k){
					if(searchinfo->rock_map[i-1][j]=='_'&&dog_move_map[i-1][j]==0){
						dog_move_map[i-1][j]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i+1][j]=='_'&&dog_move_map[i+1][j]==0){
						dog_move_map[i+1][j]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i][j-1]=='_'&&dog_move_map[i][j-1]==0){
						dog_move_map[i][j-1]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i][j+1]=='_'&&dog_move_map[i][j+1]==0){
						dog_move_map[i][j+1]=k+1;
						flag++;
					}
				}
			}
		}
		k++;
	}

	int i=0;
	while(searchinfo->dog_list[i++][1]!=0) dog_num++;

	k=1;
	while(move_dog_num<dog_num){
		int i=0;
		while(searchinfo->dog_list[i++][1]!=0){
			if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]==k&&k>1){
				if(dog_move_map[searchinfo->dog_list[i-1][1]-1][searchinfo->dog_list[i-1][2]]==k-1
					&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]-1][searchinfo->dog_list[i-1][2]]==0){
						searchinfo->dog_map[searchinfo->dog_list[i-1][1]-1][searchinfo->dog_list[i-1][2]]=1;
						searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
						searchinfo->dog_list[i-1][1]--;
				}else if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]-1]==k-1
						&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]-1]==0){
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]-1]=1;
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
							searchinfo->dog_list[i-1][2]--;
				}else if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]+1]==k-1
						&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]+1]==0){
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]+1]=1;
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
							searchinfo->dog_list[i-1][2]++;
				}else if(dog_move_map[searchinfo->dog_list[i-1][1]+1][searchinfo->dog_list[i-1][2]]==k-1
						&&searchinfo->dog_map[searchinfo->dog_list[i-1][1]+1][searchinfo->dog_list[i-1][2]]==0){
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]+1][searchinfo->dog_list[i-1][2]]=1;
							searchinfo->dog_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]=0;
							searchinfo->dog_list[i-1][1]++;
				}
				move_dog_num++;
			}
			else if(dog_move_map[searchinfo->dog_list[i-1][1]][searchinfo->dog_list[i-1][2]]==1&&k==1) move_dog_num++;
		}
		k++;if(k>100) break;
	}
}

int Search_double(struct SEARCHINFO *searchinfo, int ninja[2][2], char moves[20][20], int deep, int depth){
	int best=-10000, value, num, num_1, flag=0;
	struct SEARCHINFO _searchinfo;
	
	Copy_searchinfo(&_searchinfo, searchinfo);
	if(deep!=0){
		if(hit==3){
			if(deep%6==1||deep%6==2||deep%6==3){
				num=0;
				num_1=1;
			}else{
				num=1;
				num_1=0;
			}
		}
		else{
			if(deep%4==1||deep%4==2){
				num=0;
				num_1=1;
			}else{
				num=1;
				num_1=0;
			}
		}
		if(_searchinfo.rock_map[ninja[num][0]][ninja[num][1]]=='W') return -9999+deep;
		if(_searchinfo.rock_map[ninja[num][0]][ninja[num][1]]=='O'){
			int flag=0;
			if(flag==0&&moves[19][deep-1]=='U'
				&&_searchinfo.rock_map[ninja[num][0]-1][ninja[num][1]]=='_'
				&&_searchinfo.dog_map[ninja[num][0]-1][ninja[num][1]]==0
				&&(ninja[num][0]-1!=ninja[num_1][0]||ninja[num][1]!=ninja[num_1][1])){
					_searchinfo.rock_map[ninja[num][0]][ninja[num][1]]='_';
					_searchinfo.rock_map[ninja[num][0]-1][ninja[num][1]]='O';
					flag=1;
			}
			if(flag==0&&moves[19][deep-1]=='D'
				&&_searchinfo.rock_map[ninja[num][0]+1][ninja[num][1]]=='_'
				&&_searchinfo.dog_map[ninja[num][0]+1][ninja[num][1]]==0
				&&(ninja[num][0]+1!=ninja[num_1][0]||ninja[num][1]!=ninja[num_1][1])){
					_searchinfo.rock_map[ninja[num][0]][ninja[num][1]]='_';
					_searchinfo.rock_map[ninja[num][0]+1][ninja[num][1]]='O';
					flag=1;
			}
			if(flag==0&&moves[19][deep-1]=='L'
				&&_searchinfo.rock_map[ninja[num][0]][ninja[num][1]-1]=='_'
				&&_searchinfo.dog_map[ninja[num][0]][ninja[num][1]-1]==0
				&&(ninja[num][0]!=ninja[num_1][0]||ninja[num][1]-1!=ninja[num_1][1])){
					_searchinfo.rock_map[ninja[num][0]][ninja[num][1]]='_';
					_searchinfo.rock_map[ninja[num][0]][ninja[num][1]-1]='O';
					flag=1;
			}
			if(flag==0&&moves[19][deep-1]=='R'
				&&_searchinfo.rock_map[ninja[num][0]][ninja[num][1]+1]=='_'
				&&_searchinfo.dog_map[ninja[num][0]][ninja[num][1]+1]==0
				&&(ninja[num][0]!=ninja[num_1][0]||ninja[num][1]+1!=ninja[num_1][1])){
					_searchinfo.rock_map[ninja[num][0]][ninja[num][1]]='_';
					_searchinfo.rock_map[ninja[num][0]][ninja[num][1]+1]='O';
					flag=1;
			}
			if(flag==0) return -9999+deep;
		}

		if(_searchinfo.soul_map[ninja[num][0]][ninja[num][1]]==1){
			_searchinfo.bonus+=(10-(deep/2))*100;
			_searchinfo.soul_map[ninja[num][0]][ninja[num][1]]=0;
		}
	}

	if(((deep-hit*2)%4==0&&deep>=hit*2)||deep==hit*2){
		if(deep==hit*2&&bunsin[0]!=0&&_searchinfo.rock_map[bunsin[0]][bunsin[1]]=='_') Dog_move_single(&_searchinfo, bunsin);
		else Dog_move_double(&_searchinfo, ninja);
		if(_searchinfo.dog_map[ninja[0][0]][ninja[0][1]]!=0||_searchinfo.dog_map[ninja[1][0]][ninja[1][1]]!=0) return -9999+deep;
	}

	if(depth<=deep) return Soul_value_double(&_searchinfo, ninja)+_searchinfo.bonus;

	moves[19][deep+1]='\0';
	if(hit==3){
		if(deep%6==0||deep%6==1||deep%6==2) num=0;
		else num=1;
	}
	else{
		if(deep%4==0||deep%4==1) num=0;
		else num=1;
	}
	for(int i=0;i<5;i++){
		if(deep>=hit*2&&i==4) break;
		moves[19][deep]=move_pattern[i];
		switch(i){
			case 0:
				ninja[num][0]--;
				value=Search_double(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[num][0]++;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 1:
				ninja[num][1]++;
				value=Search_double(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[num][1]--;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 2:
				ninja[num][0]++;
				value=Search_double(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[num][0]--;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 3:
				ninja[num][1]--;
				value=Search_double(&_searchinfo, ninja, moves, deep+1, depth);
				ninja[num][1]++;
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
			case 4:
				value=Search_double(&_searchinfo, ninja, moves, deep+1, depth);
				if(best<value){
					best=value;
					if(deep+1==depth) strcpy(moves[deep], moves[19]);
					else strcpy(moves[deep], moves[deep+1]);
				}
				break;
		}
	}

	return best;
}

int Bunsin_check(struct SEARCHINFO *searchinfo, int ninja[2][2]){
	int k=1, flag=1, dog_num=0, move_dog_num=0, dog_move_map[17][14]={0};

	dog_move_map[bunsin[0]][bunsin[1]]=1;

	while(flag!=0){
		flag=0;
		for(int i=1;i<16;i++){
			for(int j=1;j<13;j++){
				if(dog_move_map[i][j]==k){
					if(searchinfo->rock_map[i-1][j]=='_'&&dog_move_map[i-1][j]==0){
						dog_move_map[i-1][j]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i+1][j]=='_'&&dog_move_map[i+1][j]==0){
						dog_move_map[i+1][j]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i][j-1]=='_'&&dog_move_map[i][j-1]==0){
						dog_move_map[i][j-1]=k+1;
						flag++;
					}
					if(searchinfo->rock_map[i][j+1]=='_'&&dog_move_map[i][j+1]==0){
						dog_move_map[i][j+1]=k+1;
						flag++;
					}
				}
			}
		}
		k++;
	}

	if(dog_move_map[ninja[0][0]][ninja[0][1]]==0||dog_move_map[ninja[1][0]][ninja[1][1]]==0) return -1;
	return 0;
}

void think(){
	int value_0, value_1, value_2, value_if, flag, dog_num=0, moves_x=0, moves_y=0;
	char moves_0[20][20]={""}, moves_1[20][20]={""}, moves_2[20][20]={""}, moves_null[20][20]={""};
	struct SEARCHINFO searchinfo_my, searchinfo_my_dog, searchinfo_enemy;

	Init_searchinfo(&searchinfo_my, 0);
	Init_searchinfo(&searchinfo_enemy, 1);

	hit=2;
	flag=0;
	bunsin[0]=0;
	if((fabs(float(gameinfo_my.ninja[0][0]-gameinfo_my.ninja[1][0]))+fabs(float(gameinfo_my.ninja[0][1]-gameinfo_my.ninja[1][1])))>5){
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(searchinfo_my.dog_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]==1) dog_num++;
			}
		}
		if(gameinfo_my.cost[7]<=gameinfo_my.ninryoku&&gameinfo_my.cost[7]<=dog_num*4){
			Init_searchinfo(&searchinfo_my_dog, 0);
			for(int i=-1;i<2;i++){
				for(int j=-1;j<2;j++){
					searchinfo_my_dog.dog_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]=0;
				}
			}
			value_0=Search_single(&searchinfo_my_dog, gameinfo_my.ninja[0], moves_0, 0, 8);
			if(value_0<-9000) flag=1;
			value_1=Search_single(&searchinfo_my_dog, gameinfo_my.ninja[1], moves_1, 0, 8);
			if(value_1<-9000) flag+=2;
			if(flag==0){
				printf("3\n");
				printf("7 0\n");
				printf("%s\n", moves_0[0]);
				printf("%s\n", moves_1[0]);
				return;
			}
			
		}
		flag=0;
		dog_num=0;
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(searchinfo_my.dog_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]==1) dog_num++;
			}
		}
		if(gameinfo_my.cost[7]<=gameinfo_my.ninryoku&&gameinfo_my.cost[7]<=dog_num*4){
			Init_searchinfo(&searchinfo_my_dog, 0);
			for(int i=-1;i<2;i++){
				for(int j=-1;j<2;j++){
					searchinfo_my_dog.dog_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]=0;
				}
			}
			value_0=Search_single(&searchinfo_my_dog, gameinfo_my.ninja[0], moves_0, 0, 8);
			if(value_0<-9000) flag=1;
			value_1=Search_single(&searchinfo_my_dog, gameinfo_my.ninja[1], moves_1, 0, 8);
			if(value_1<-9000) flag+=2;
			if(flag==0){
				printf("3\n");
				printf("7 1\n");
				printf("%s\n", moves_0[0]);
				printf("%s\n", moves_1[0]);
				return;
			}
		}
		flag=0;
		if(gameinfo_my.cost[2]<=gameinfo_enemy.ninryoku&&-9000<Search_double(&searchinfo_enemy, gameinfo_enemy.ninja, moves_null, 0, 4)){
			for(int k=0;k<2;k++){
				for(int i=-2;i<3;i++){
					for(int j=-2;j<3;j++){
						if(0<(gameinfo_my.ninja[k][0]+i)&&(gameinfo_my.ninja[k][0]+i)<17&&0<(gameinfo_my.ninja[k][1]+j)&&(gameinfo_my.ninja[k][1]+j)<14
							&&searchinfo_my.rock_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]=='_'
							&&searchinfo_my.dog_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]==0
							&&(i!=0||j!=0)){
								searchinfo_my.rock_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]='O';
								if(k==0) value_if=Search_single(&searchinfo_my, gameinfo_my.ninja[0], moves_null, 0, 2);
								else value_if=Search_single(&searchinfo_my, gameinfo_my.ninja[1], moves_null, 0, 2);
								if(value_if<-9000){
									value_0=Search_single(&searchinfo_my, gameinfo_my.ninja[0], moves_0, 0, 8);
									if(value_0<100) flag=1;
									value_1=Search_single(&searchinfo_my, gameinfo_my.ninja[1], moves_1, 0, 8);
									if(value_1<100) flag+=2;
									if(gameinfo_my.ninryoku>=gameinfo_my.cost[5]&&flag!=0){
										value_if=value_0+1000;
				for(int i=6;i>=0;i-=2){
					bunsin[0]=bunsin_pattern[i];
					bunsin[1]=bunsin_pattern[i+1];
					if(searchinfo_my.rock_map[bunsin[0]][bunsin[1]]=='_'){
						value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_null, 0, 8);
						if(value_2>value_if){
							value_if=value_2;
							bunsin_if[0]=bunsin[0];
							bunsin_if[1]=bunsin[1];
							strcpy(moves_2[0], moves_null[0]);
							if(moves_2[0][0]=='\0'){
								for(int i=6;i>0;i-=2){
									value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, i);
									if(moves_2[0][0]!='\0') break;
								}
							}
						}
					}
				}
				if(value_if>value_0+1000){
					printf("3\n");
					printf("5 %d %d\n", bunsin_if[0], bunsin_if[1]);
					printf("%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
					printf("%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
					return;
				}
									}
									bunsin[0]=0;
									if(gameinfo_my.ninryoku>=gameinfo_my.cost[3]&&flag==1){
										for(int i=-2;i<3;i++){
											for(int j=-2;j<3;j++){
												if(0<(gameinfo_my.ninja[0][0]+i)&&(gameinfo_my.ninja[0][0]+i)<17&&0<(gameinfo_my.ninja[0][1]+j)&&(gameinfo_my.ninja[0][1]+j)<14
													&&searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]=='O'
													&&(i!=-2||j!=-2)&&(i!=2||j!=-2)&&(i!=-2||j!=2)&&(i!=2||j!=2)){
														searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='_';
														value_2=Search_single(&searchinfo_my, gameinfo_my.ninja[0], moves_2, 0, 4);
														searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='O';
														if(value_2>value_0+1000){
															printf("3\n");
															printf("3 %d %d\n", gameinfo_my.ninja[0][0]+i, gameinfo_my.ninja[0][1]+j);
															printf("%s\n", moves_2[0]);
															printf("%s\n", moves_1[0]);
															return;
														}
												}
											}
										}
									}
									if(gameinfo_my.ninryoku>=gameinfo_my.cost[3]&&flag==2){
										for(int i=-2;i<3;i++){
											for(int j=-2;j<3;j++){
												if(0<(gameinfo_my.ninja[1][0]+i)&&(gameinfo_my.ninja[1][0]+i)<17&&0<(gameinfo_my.ninja[1][1]+j)&&(gameinfo_my.ninja[1][1]+j)<14
													&&searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]=='O'
													&&(i!=-2||j!=-2)&&(i!=2||j!=-2)&&(i!=-2||j!=2)&&(i!=2||j!=2)){
														searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='_';
														value_2=Search_single(&searchinfo_my, gameinfo_my.ninja[1], moves_2, 0, 4);
														searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='O';
														if(value_2>value_1+1000){
															printf("3\n");
															printf("3 %d %d\n", gameinfo_my.ninja[1][0]+i, gameinfo_my.ninja[1][1]+j);
															printf("%s\n", moves_0[0]);
															printf("%s\n", moves_2[0]);
															return;
														}
												}
											}
										}
									}
								}
								searchinfo_my.rock_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]='_';
						}
					}
				}
			}
		}
		value_0=Search_single(&searchinfo_my, gameinfo_my.ninja[0], moves_0, 0, 8);
		if(value_0<100) flag=1;
		value_1=Search_single(&searchinfo_my, gameinfo_my.ninja[1], moves_1, 0, 8);
		if(value_1<100) flag+=2;
		if(gameinfo_my.ninryoku>=gameinfo_my.cost[6]&&value_0>-9000&&value_1>-9000){
			value_if=Search_single(&searchinfo_enemy, gameinfo_enemy.ninja[0], moves_null, 0, 2);
			if(value_if<-9000){
				printf("3\n");
				printf("6 %d %d\n", gameinfo_enemy.ninja[0][0], gameinfo_enemy.ninja[0][1]);
				printf("%s\n", moves_0[0]);
				printf("%s\n", moves_1[0]);
				return;
			}
			value_if=Search_single(&searchinfo_enemy, gameinfo_enemy.ninja[1], moves_null, 0, 2);
			if(value_if<-9000){
				printf("3\n");
				printf("6 %d %d\n", gameinfo_enemy.ninja[1][0], gameinfo_enemy.ninja[1][1]);
				printf("%s\n", moves_0[0]);
				printf("%s\n", moves_1[0]);
				return;
			}
		}
		if(gameinfo_my.ninryoku>=gameinfo_my.cost[5]&&flag!=0){
			value_if=value_0+1000;
				for(int i=6;i>=0;i-=2){
					bunsin[0]=bunsin_pattern[i];
					bunsin[1]=bunsin_pattern[i+1];
					if(searchinfo_my.rock_map[bunsin[0]][bunsin[1]]=='_'){
						value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_null, 0, 8);
						if(value_2>value_if){
							value_if=value_2;
							bunsin_if[0]=bunsin[0];
							bunsin_if[1]=bunsin[1];
							strcpy(moves_2[0], moves_null[0]);
							if(moves_2[0][0]=='\0'){
								for(int i=6;i>0;i-=2){
									value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, i);
									if(moves_2[0][0]!='\0') break;
								}
							}
						}
					}
				}
				if(value_if>value_0+1000){
					printf("3\n");
					printf("5 %d %d\n", bunsin_if[0], bunsin_if[1]);
					printf("%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
					printf("%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
					return;
				}
		}
		bunsin[0]=0;
		if(gameinfo_my.ninryoku>=gameinfo_my.cost[3]&&flag==1){
			for(int i=-2;i<3;i++){
				for(int j=-2;j<3;j++){
					if(0<(gameinfo_my.ninja[0][0]+i)&&(gameinfo_my.ninja[0][0]+i)<17&&0<(gameinfo_my.ninja[0][1]+j)&&(gameinfo_my.ninja[0][1]+j)<14
						&&searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]=='O'
						&&(i!=-2||j!=-2)&&(i!=2||j!=-2)&&(i!=-2||j!=2)&&(i!=2||j!=2)){
							searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='_';
							value_2=Search_single(&searchinfo_my, gameinfo_my.ninja[0], moves_2, 0, 8);
							searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='O';
							if(value_2>value_0+1000){
								printf("3\n");
								printf("3 %d %d\n", gameinfo_my.ninja[0][0]+i, gameinfo_my.ninja[0][1]+j);
								printf("%s\n", moves_2[0]);
								printf("%s\n", moves_1[0]);
								return;
							}
					}
				}
			}
		}
		if(gameinfo_my.ninryoku>=gameinfo_my.cost[3]&&flag==2){
			for(int i=-2;i<3;i++){
				for(int j=-2;j<3;j++){
					if(0<(gameinfo_my.ninja[1][0]+i)&&(gameinfo_my.ninja[1][0]+i)<17&&0<(gameinfo_my.ninja[1][1]+j)&&(gameinfo_my.ninja[1][1]+j)<14
						&&searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]=='O'
						&&(i!=-2||j!=-2)&&(i!=2||j!=-2)&&(i!=-2||j!=2)&&(i!=2||j!=2)){
							searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='_';
							value_2=Search_single(&searchinfo_my, gameinfo_my.ninja[1], moves_2, 0, 8);
							searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='O';
							if(value_2>value_1+1000){
								printf("3\n");
								printf("3 %d %d\n", gameinfo_my.ninja[1][0]+i, gameinfo_my.ninja[1][1]+j);
								printf("%s\n", moves_0[0]);
								printf("%s\n", moves_2[0]);
								return;
							}
					}
				}
			}
		}
		hit=3;
		if((value_0>=400||value_1>=400)&&gameinfo_my.ninryoku>=gameinfo_my.cost[0]&&gameinfo_my.cost[0]==1&&gameinfo_my.cost[7]>12){
			value_0=Search_single(&searchinfo_my, gameinfo_my.ninja[0], moves_0, 0, 8);
			value_1=Search_single(&searchinfo_my, gameinfo_my.ninja[1], moves_1, 0, 8);
			printf("3\n");
			printf("0\n");
			printf("%s\n", moves_0[0]);
			printf("%s\n", moves_1[0]);
			return;
		}
		printf("2\n");
		printf("%s\n", moves_0[0]);
		printf("%s\n", moves_1[0]);
	}else{
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(searchinfo_my.dog_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]==1) dog_num++;
			}
		}
		if(gameinfo_my.cost[7]<=gameinfo_my.ninryoku&&gameinfo_my.cost[7]<=dog_num*4){
			Init_searchinfo(&searchinfo_my_dog, 0);
			for(int i=-1;i<2;i++){
				for(int j=-1;j<2;j++){
					searchinfo_my_dog.dog_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]=0;
				}
			}
			value_0=Search_double(&searchinfo_my_dog, gameinfo_my.ninja, moves_2, 0, 8);
			if(value_0>-9000){
				printf("3\n");
				printf("7 0\n");
				printf("%c%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
				printf("%c%c%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
				return;
			}
		}
		dog_num=0;
		for(int i=-1;i<2;i++){
			for(int j=-1;j<2;j++){
				if(searchinfo_my.dog_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]==1) dog_num++;
			}
		}
		if(gameinfo_my.cost[7]<=gameinfo_my.ninryoku&&gameinfo_my.cost[7]<=dog_num*4){
			Init_searchinfo(&searchinfo_my_dog, 0);
			for(int i=-1;i<2;i++){
				for(int j=-1;j<2;j++){
					searchinfo_my_dog.dog_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]=0;
				}
			}
			value_0=Search_double(&searchinfo_my_dog, gameinfo_my.ninja, moves_2, 0, 8);
			if(value_0>-9000){
				printf("3\n");
				printf("7 1\n");
				printf("%c%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
				printf("%c%c%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
				return;
			}
		}
		if(gameinfo_my.cost[2]<=gameinfo_enemy.ninryoku&&-9000<Search_double(&searchinfo_enemy, gameinfo_enemy.ninja, moves_null, 0, 4)){
			for(int k=0;k<2;k++){
				for(int i=-2;i<3;i++){
					for(int j=-2;j<3;j++){
						if(0<(gameinfo_my.ninja[k][0]+i)&&(gameinfo_my.ninja[k][0]+i)<17&&0<(gameinfo_my.ninja[k][1]+j)&&(gameinfo_my.ninja[k][1]+j)<14
							&&searchinfo_my.rock_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]=='_'
							&&searchinfo_my.dog_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]==0
							&&(i!=0||j!=0)){
								searchinfo_my.rock_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]='O';
								value_if=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_null, 0, 4);
								if(value_if<-9000){
									value_0=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_0, 0, 8);
									if(value_0<200){
										if(gameinfo_my.ninryoku>=gameinfo_my.cost[5]){
											value_if=value_0+1000;
											for(int i=6;i>=0;i-=2){
												bunsin[0]=bunsin_pattern[i];
												bunsin[1]=bunsin_pattern[i+1];
												if(searchinfo_my.rock_map[bunsin[0]][bunsin[1]]=='_'){
													value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_null, 0, 8);
													if(value_2>value_if){
														value_if=value_2;
														bunsin_if[0]=bunsin[0];
														bunsin_if[1]=bunsin[1];
														strcpy(moves_2[0], moves_null[0]);
														if(moves_2[0][0]=='\0'){
															for(int i=6;i>0;i-=2){
																value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, i);
																if(moves_2[0][0]!='\0') break;
															}
														}
													}
												}
											}
											if(value_if>value_0+1000){
												printf("3\n");
												printf("5 %d %d\n", bunsin_if[0], bunsin_if[1]);
												printf("%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
												printf("%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
												return;
											}
										}
										bunsin[0]=0;
										if(gameinfo_my.ninryoku>=gameinfo_my.cost[3]){
											for(int i=-2;i<3;i++){
												for(int j=-2;j<3;j++){
													if(0<(gameinfo_my.ninja[0][0]+i)&&(gameinfo_my.ninja[0][0]+i)<17&&0<(gameinfo_my.ninja[0][1]+j)&&(gameinfo_my.ninja[0][0]+j)<14
														&&searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]=='O'
														&&(i!=-2||j!=-2)&&(i!=2||j!=-2)&&(i!=-2||j!=2)&&(i!=2||j!=2)){
															searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='_';
															value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, 4);
															searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='O';
															if(value_2>value_0+1000){
																printf("3\n");
																printf("3 %d %d\n", gameinfo_my.ninja[0][0]+i, gameinfo_my.ninja[0][1]+j);
																printf("%c%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
																printf("%c%c%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
																return;
															}
													}
												}
											}
											for(int i=-2;i<3;i++){
												for(int j=-2;j<3;j++){
													if(0<(gameinfo_my.ninja[1][0]+i)&&(gameinfo_my.ninja[1][0]+i)<17&&0<(gameinfo_my.ninja[1][1]+j)&&(gameinfo_my.ninja[1][0]+j)<14
														&&searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]=='O'
														&&(i!=-2||j!=-2)&&(i!=2||j!=-2)&&(i!=-2||j!=2)&&(i!=2||j!=2)){
															searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='_';
															value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, 4);
															searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='O';
															if(value_2>value_0+1000){
																printf("3\n");
																printf("3 %d %d\n", gameinfo_my.ninja[1][0]+i, gameinfo_my.ninja[1][1]+j);
																printf("%c%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
																printf("%c%c%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
																return;
															}
													}
												}
											}
										}
									}
								}
								searchinfo_my.rock_map[gameinfo_my.ninja[k][0]+i][gameinfo_my.ninja[k][1]+j]='_';
						}
					}
				}
			}
		}
		value_0=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_0, 0, 8);
		if(gameinfo_my.ninryoku>=gameinfo_my.cost[6]&&value_0>-9000){
			value_if=Search_single(&searchinfo_enemy, gameinfo_enemy.ninja[0], moves_null, 0, 2);
			if(value_if<-9000){
				printf("3\n");
				printf("6 %d %d\n", gameinfo_enemy.ninja[0][0], gameinfo_enemy.ninja[0][1]);
				printf("%c%c%c%c\n", moves_0[0][0], moves_0[0][1], moves_0[0][4], moves_0[0][5]);
				printf("%c%c%c%c\n", moves_0[0][2], moves_0[0][3], moves_0[0][6], moves_0[0][7]);
				return;
			}
			value_if=Search_single(&searchinfo_enemy, gameinfo_enemy.ninja[1], moves_null, 0, 2);
			if(value_if<-9000){
				printf("3\n");
				printf("6 %d %d\n", gameinfo_enemy.ninja[1][0], gameinfo_enemy.ninja[1][1]);
				printf("%c%c%c%c\n", moves_0[0][0], moves_0[0][1], moves_0[0][4], moves_0[0][5]);
				printf("%c%c%c%c\n", moves_0[0][2], moves_0[0][3], moves_0[0][6], moves_0[0][7]);
				return;
			}
		}
		if(value_0<200){
			if(gameinfo_my.ninryoku>=gameinfo_my.cost[5]){
				value_if=value_0+1000;
				for(int i=6;i>=0;i-=2){
					bunsin[0]=bunsin_pattern[i];
					bunsin[1]=bunsin_pattern[i+1];
					if(searchinfo_my.rock_map[bunsin[0]][bunsin[1]]=='_'){
						value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_null, 0, 8);
						if(value_2>value_if){
							value_if=value_2;
							bunsin_if[0]=bunsin[0];
							bunsin_if[1]=bunsin[1];
							strcpy(moves_2[0], moves_null[0]);
							if(moves_2[0][0]=='\0'){
								for(int i=6;i>0;i-=2){
									value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, i);
									if(moves_2[0][0]!='\0') break;
								}
							}
						}
					}
				}
				if(value_if>value_0+1000){
					printf("3\n");
					printf("5 %d %d\n", bunsin_if[0], bunsin_if[1]);
					printf("%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
					printf("%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
					return;
				}
			}
			bunsin[0]=0;
			if(gameinfo_my.ninryoku>=gameinfo_my.cost[3]){
				for(int i=-1;i<2;i++){
					for(int j=-1;j<2;j++){
						if(0<(gameinfo_my.ninja[0][0]+i)&&(gameinfo_my.ninja[0][0]+i)<17&&0<(gameinfo_my.ninja[0][1]+j)&&(gameinfo_my.ninja[0][0]+j)<14
							&&searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]=='O'){
								searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='_';
								value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, 8);
								searchinfo_my.rock_map[gameinfo_my.ninja[0][0]+i][gameinfo_my.ninja[0][1]+j]='O';
								if(value_2>value_0+1000){
									printf("3\n");
									printf("3 %d %d\n", gameinfo_my.ninja[0][0]+i, gameinfo_my.ninja[0][1]+j);
									printf("%c%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
									printf("%c%c%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
									return;
								}
						}
					}
				}
				/*for(int i=-2;i<3;i++){
					for(int j=-2;j<3;j++){
						if(0<(gameinfo_my.ninja[1][0]+i)&&(gameinfo_my.ninja[1][0]+i)<17&&0<(gameinfo_my.ninja[1][1]+j)&&(gameinfo_my.ninja[1][0]+j)<14
							&&searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]=='O'){
								searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='_';
								value_2=Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, 8);
								searchinfo_my.rock_map[gameinfo_my.ninja[1][0]+i][gameinfo_my.ninja[1][1]+j]='O';
								if(value_2>value_0+1000){
									printf("3\n");
									printf("3 %d %d\n", gameinfo_my.ninja[1][0]+i, gameinfo_my.ninja[1][1]+j);
									printf("%c%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][4], moves_2[0][5]);
									printf("%c%c%c%c\n", moves_2[0][2], moves_2[0][3], moves_2[0][6], moves_2[0][7]);
									return;
								}
						}
					}
				}*/
			}
		}
		hit=3;
		bunsin[0]=0;
		if(value_0>=500&&gameinfo_my.ninryoku>=gameinfo_my.cost[0]&&gameinfo_my.cost[0]==1&&gameinfo_my.cost[7]>12){
			Search_double(&searchinfo_my, gameinfo_my.ninja, moves_2, 0, 8);
			printf("3\n");
			printf("0\n");
			printf("%c%c%c\n", moves_2[0][0], moves_2[0][1], moves_2[0][2]);
			printf("%c%c%c\n", moves_2[0][3], moves_2[0][4], moves_2[0][5]);
			return;
		}
		printf("2\n");
		printf("%c%c%c%c\n", moves_0[0][0], moves_0[0][1], moves_0[0][4], moves_0[0][5]);
		printf("%c%c%c%c\n", moves_0[0][2], moves_0[0][3], moves_0[0][6], moves_0[0][7]);
	}
}

void clear(){
	int i;

	for(i=0;i<100;i++){
		gameinfo_my.ninken[i][0]=0;
		gameinfo_my.ninken[i][1]=0;
		gameinfo_my.soul[i][0]=0;
		gameinfo_my.soul[i][1]=0;
		gameinfo_enemy.ninken[i][0]=0;
		gameinfo_enemy.ninken[i][1]=0;
		gameinfo_enemy.soul[i][0]=0;
		gameinfo_enemy.soul[i][1]=0;
	}
}

int main(){
	// AI‚Ì–¼‘O‚ðo—Í
	
	cout << "Super_Brokoron" << endl;
	cout.flush();
	clear();

	while(input()){
		think();
		clear();
		cout.flush();
	}

	return 0;
}
