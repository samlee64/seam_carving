create table files
( name varchar primary key unique not null,
  metadata jsonb not null,
  comment varchar
);

alter table executions add column output_files text[];
