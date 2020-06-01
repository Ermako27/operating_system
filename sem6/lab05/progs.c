#include <stdio.h>

#include <fcntl.h>

int main()
{
	int fd = open("alphabet.txt",O_RDONLY);
	FILE *fs1 = fdopen(fd,"r");
	char buff1[20];
	setvbuf(fs1,buff1,_IOFBF,20);
	FILE *fs2 = fdopen(fd,"r");
	char buff2[20];
	setvbuf(fs2,buff2,_IOFBF,20);
	int flag1 = 1, flag2 = 2;

	while(flag1 == 1 || flag2 == 1)
	{
		char c;
		flag1 = fscanf(fs1,"%c",&c);
		if (flag1 == 1) { fprintf(stdout,"%c",c); }
		flag2 = fscanf(fs2,"%c",&c);
		if (flag2 == 1) { fprintf(stdout,"%c",c); }
	}
	return 0;



const fastify = require('fastify')({
  logger: false
});
import userRouter from './routers/UserRoutes.js';
import forumRouter from './routers/ForumRoutes.js';
import threadRouter from './routers/ThreadRoutes.js';
import postRouter from './routers/PostRoutes.js';
import serviceRouter from './routers/ServiceRoutes.js';
import ServiceModel from './models/ServiceModel.js';
fastify.register(userRouter);
fastify.register(forumRouter);
fastify.register(threadRouter);
fastify.register(postRouter);
fastify.register(serviceRouter);
const port = process.env.PORT || 8001;
fastify.post('/api/service/clear', async (request, reply) => {
        // console.log(req.originalUrl, req.method);
        try {   
                await ServiceModel.clearAll();
        } catch (error) {
                console.log('--------------------------------------------');
                console.log(error);
                console.log('ERROR IN COUNTING');
                return reply.status(500).send({ message : "crash" });
        }
        reply.type('application/json').status(200);
        return null;
});
fastify.addContentTypeParser('application/json', { parseAs: 'string' }, function (request, body, done) {
    try {
      var json = JSON.parse(body)
      done(null, json)
    } catch (err) {
      done(null, undefined);
    }
  });
fastify.listen(port, '0.0.0.0',(err,address) => {
        console.log(`Server listening on port ${port}`);
});