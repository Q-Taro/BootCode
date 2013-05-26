/* Copyright (c) 2000 by Tsuyoshi Ishii. All rights reserved. ---------------*/
/* must lock if try to communicate another task -----------------------------*/
static comm_lk = 0;
void
comm_lock()
{
	smp_lock(&comm_lk);
}

void
comm_unlock()
{
	smp_unlock(&comm_lk);
}
