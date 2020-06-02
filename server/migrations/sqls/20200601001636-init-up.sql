create type status as ENUM('executing', 'uploading', 'done', 'error');

create extension "uuid-ossp";

create table executions (
  id uuid primary key not null default uuid_generate_v4(),
  image_name varchar not null,
  routine varchar not null,
  s3_url varchar,
  status status not null default 'executing',
  params jsonb
)
