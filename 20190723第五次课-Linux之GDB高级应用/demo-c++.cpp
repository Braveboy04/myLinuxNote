class point  
{  
  public:  
        point(int x, int y)  
        {  
                this->x = x;  
                this->y = y;  
                ins_cnt++;  
        }  
        ~point()  
        {  
                ins_cnt--;  
        }  
        static int get_instances()  
        {  
                return ins_cnt;  
        }  
        point & move(int addx, int addy)  
        {  
                this->x += addx;  
                this->y += addy;  
                return *this;  
        }  
  private:  
        int x, y;  
        static int ins_cnt;  
};  
int point::ins_cnt = 0;  
int main()  
{  
        point x(3, 4);  
        x.move(10, 10);  
        int p_ins_cnt = point::get_instances();  
        return 0;  
}