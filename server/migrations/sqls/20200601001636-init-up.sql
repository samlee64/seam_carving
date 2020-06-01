create type status as ENUM('executing', 'uploading', 'done', 'error');

create table executions (
  id varchar primary key not null,
  image_name varchar not null,
  routine varchar not null,
  s3_url varchar,
  status status not null default 'executing',
  params jsonb
)
