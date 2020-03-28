"""
#include <pthread.h>  
int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr); 成功返回0，其它返回值表示出错  
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock ); 成功返回0，其它返回值表示出错  
//在指定的时间之前在读写锁上获取写锁（写锁定）
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock, const struct timespec *abs_timeout); 成功返回0，其它返回值表示出错  
int pthread_rwlock_reltimedrdlock_np(pthread_rwlock_t *rwlock, const struct timespec *abs_timeout); 成功返回0，其它返回值表示出错  
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock); 成功返回0，其它返回值表示出错  
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock ); 成功返回0，其它返回值表示出错  
int  pthread_rwlock_timedwrlock(pthread_rwlock_t  *rwlock, const struct timespec *abs_timeout); 成功返回0，其它返回值表示出错  
int pthread_rwlock_reltimedwrlock_np(pthread_rwlock_t *rwlock, const struct timespec *abs_timeout); 成功返回0，其它返回值表示出错  
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock); 成功返回0，其它返回值表示出错  
int pthread_rwlock_unlock (pthread_rwlock_t *rwlock); 成功返回0，其它返回值表示出错  
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock); 成功返回0，其它返回值表示出错  
int pthread_rwlockattr_init(pthread_rwlockattr_t *attr); 成功返回0，其它返回值表示出错  
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr); 成功返回0，其它返回值表示出错  
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared); 成功返回0，其它返回值表示出错  
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *pshared); 成功返回0，其它返回值表示出错 
"""
